/******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
* File Name    : r_usbh1_hhid_main.c
* Description  : USB Host MSC application code
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_usb_hhid_apl.h"
#include "r_usbh1_hhid_config.h"
#include "r_usbh1_basic_if.h"
#include "r_usbh1_basic_local.h"
#include "r_usbh1_rtos.h"

#include "r_usb_hhid_control.h"
#include "r_usb_hhid_dev.h"

#include "r_os_abstraction_api.h"
#include "driver.h"
#include "iodefine.h"

#include "tp_if.h"
#include "tp.h"

#include "debug.h"

/******************************************************************************
Constant macro definitions
******************************************************************************/
#define USB_KEY_INPUT_BUFF_NUM  (12)
/* ASCII CODE */
#define ASCII_LF                (0x0A)
#define ASCII_CR                (0x0D)
#define ASCII_0                 ('0')
#define ASCII_1                 ('1')

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

uint8_t     usbh1_drive_search_lock;
DEV_INFO_t  usbh1_hid_dev_info[USBH1_MAXDEVADDR];    /* MSC device control structure */

const uint16_t g_usbh1_hhid_devicetpl[] =       /* TPL(Targeted Peripheral List) */
{
    1,                      /* Number of tpl table */
    0,                      /* Reserved */
    USBH1_NOVENDOR,         /* Vendor ID  : no-check */
    USBH1_NOPRODUCT,        /* Product ID : no-check */
};

uint16_t          g_usbh1_msg_check;
volatile uint8_t  g_device = USBH1_HID_OTHER;

static usbh1_setup_t    usbh1_shhid_class_request_setup __attribute__ ((section ("USB_CLASS_1")));                   /* Control Transfer Request field */
static st_usbh1_utr_t   usbh1_shhid_class_control __attribute__ ((section ("USB_CLASS_1")));                         /* Request for HID to USB Request */
static st_usbh1_utr_t   usbh1_shhid_data_control __attribute__ ((section ("USB_CLASS_1")));                          /* Request for HID to USB Request */
static st_usbh1_utr_t   usbh1_ghhid_transfer_message[USBH1_MAXPIPE]  __attribute__ ((section ("USB_CLASS_1")));
USBH1_HHID_CLASS_REQUEST_PARM_t   usbh1_ghhid_class_request_parm __attribute__ ((section ("USB_CLASS_1")));          /* Host HID Class Request Parametor */
static uint8_t          g_usbh1_buf[USB_BUF_LEN] __attribute__ ((section ("USB_CLASS_1")));

void          usbh1_hid_class_request_complete(st_usbh1_utr_t *utr);
usbh1_er_t    usbh1_data_read (uint16_t devadr, uint8_t *buf);
usbh1_err_t   usbh1_hid_set_protocol( uint16_t devadr, uint8_t protocol, usbh1_utr_cb_t complete);
void          usbh1_hid_registration(void);
void          usbh1_hhid_read_complete (st_usbh1_utr_t *mess);
void          usbh1_hid_event_set(uint16_t devadr, uint16_t event );
uint16_t      usbh1_hid_event_get(uint16_t devadr);
static void   usbh1_hhid_info_set_touch (uint16_t drvnum);
static void   usbh1_hhid_info_set_mouse (uint16_t drvnum);
static void   usbh1_hhid_info_set_keyboard (uint16_t drvnum);

extern bool_t      g_task_start_flag;

/******************************************************************************
Function Name   : usbh1_hid_main
Description     : Host MSC application main process
Arguments       : uint8_t   *usb_data   : USB buffer pointer for application
Return value    : none
******************************************************************************/
void usbh1_hid_main(uint8_t *usb_data)
{
    uint16_t            ret_val = 0;
    uint16_t            event;
    uint16_t            drvnum;
    LCDEVT_ENTRY        *psEvt;
    uint8_t             device;
    static uint32_t     no_event_cnt = 0;
    /* release from touch panel information flag */
    static bool_t       release_flag = false;
    
    for(drvnum = 0; drvnum < USBH1_MAXDEVADDR; drvnum++)
    {
        event = usbh1_hid_event_get(drvnum);
            /* Check application state */
            switch (event)
            {
            case USBH1_STS_CONFIGURED :
                no_event_cnt = 0;
                device = usbh1_hhid_get_hid_protocol(drvnum);
                if ((USBH1_HID_KEYBOARD == device) || (USBH1_HID_MOUSE == device))
                {
                    usbh1_hid_set_protocol( drvnum, BOOT_PROTOCOL, &usbh1_hid_class_request_complete );
                }
                else
                {
                    usbh1_data_read (drvnum, &g_usbh1_buf);
                    /* set touch protocol (temporary defined by other on driver) */
                    usbh1_hhid_info_set_touch(drvnum);
                }
            break;

            case USBH1_STS_READ_COMPLETE :
                no_event_cnt = 0;
                usbh1_data_read (drvnum, &g_usbh1_buf);
                
                if (true == g_task_start_flag)
                {
                    if (USB_IF_TOUCH == gs_usb_dev_info[drvnum].interface)
                    {
                        /* push touch panel */
                        release_flag = false;
                    }
                    ret_val = USB_WaitSyncBuffer();
                    if (0 != ret_val)
                    {
                        DBG_printf_ERR("[ERROR] semaphore error for data\n");
                    }
                    else
                    {
                        memcpy(usb_data, &g_usbh1_buf, gs_usb_dev_info[drvnum].copy_data_len);
                        USB_ReleaseSyncBuffer();
                        
                        /* send event to LCD_Task */
                        psEvt = USB_GetEventTable( 0 );
                        if( psEvt->mode == LCDEVT_ENTRY_TP )
                        {
                            psEvt->function( drvnum );
                        }
                    }
                }
            break;

            case USBH1_STS_REQUEST_COMPLETE :
                no_event_cnt = 0;
                usbh1_data_read (drvnum, &g_usbh1_buf);
                device = usbh1_hhid_get_hid_protocol(drvnum);
                if (USBH1_HID_MOUSE == device)
                {
                    usbh1_hhid_info_set_mouse(drvnum);
                }
                else /* (USBH1_HID_KEYBOARD == device) */
                {
                    usbh1_hhid_info_set_keyboard(drvnum);
                }
                
            break;

            default :
                /* check touch event */
                if (USB_IF_TOUCH == gs_usb_dev_info[drvnum].interface)
                {
                    if (true == g_task_start_flag)
                    {
                        if ((USB_NO_EVENT_TIME / USB_TASK_DELAY) <= no_event_cnt)
                        {
                            if (false == release_flag)
                            {
                                /* touch panel release */
                                release_flag = true;
                                ret_val = USB_WaitSyncBuffer();
                                if (0 != ret_val)
                                {
                                    DBG_printf_ERR("[ERROR] semaphore error for data\n");
                                }
                                else
                                {
                                    /* no event, start touch panel task with no finger data */
                                    *(usb_data + gs_usb_dev_info[drvnum].touch->finger_info_pos) = 0;
                                    USB_ReleaseSyncBuffer();
                                    
                                    /* send event to LCD_Task */
                                    psEvt = USB_GetEventTable( 0 );
                                    if( psEvt->mode == LCDEVT_ENTRY_TP )
                                    {
                                        psEvt->function( (uint32_t)drvnum );
                                    }
                                }
                            }
                        }
                        else
                        {
                            no_event_cnt++;
                        }
                    }
                }
            break;
            }
    }

    return ;
}   /* End of function usbh1_hid_main() */

/******************************************************************************
Function Name   : usbh1_cstd_PortInit
Description     : Port Initialize
Argument        : none
Return          : none
******************************************************************************/
void usbh1_cstd_PortInit(void)
{
    /* SW3 : IRQ6 */
    PORTJ.PDR.BIT.PDR1 = 0x0;
    PORTJ.PMR.BIT.PMR1 = 0x0;
//    GPIO.PJ1PFS.BIT.PSEL = 0x06;   /* ISEL = b'0, PSEL = b'110 */

    /* VBUSEN1 : PC5 *//* OVRCUR1 : P75 */
    /* ---- PFS Register Write Protect : OFF  ---- */
    GPIO.PWPR.BIT.B0WI = 0;
    GPIO.PWPR.BIT.PFSWE = 1;

    GPIO.PC5PFS.BIT.PSEL = 0x01;   /* ISEL = b'0, PSEL = b'001 */
    GPIO.P75PFS.BIT.PSEL = 0x05;   /* ISEL = b'0, PSEL = b'101 */

    /* ---- PFS Register Write Protect : ON ---- */
    GPIO.PWPR.BIT.PFSWE = 0x0;
    GPIO.PWPR.BIT.B0WI = 0x1;

    PORTC.PMR.BIT.PMR5 = 0x1;
    PORT7.PMR.BIT.PMR5 = 0x1;
} /* End of function usbh1_cstd_PortInit() */

/******************************************************************************
* Function Name: usbh1_ahb_pci_bridge_init
* Description  :
* Arguments    : none
* Return Value : none
******************************************************************************/
void usbh1_ahb_pci_bridge_init(void)
{

    USB10.INT_ENABLE.BIT.WAKEON_INTEN   = 1;
    USB10.INT_ENABLE.BIT.UCOM_INTEN     = 1;
    USB10.INT_ENABLE.BIT.USBH_INTBEN    = 1;
    USB10.INT_ENABLE.BIT.USBH_INTAEN    = 1;
    USB10.INT_ENABLE.BIT.AHB_INTEN      = 1;

    USB10.AHB_BUS_CTR.BIT.PROT_TYPE     = 0;
    USB10.AHB_BUS_CTR.BIT.PROT_MODE     = 0;
    USB10.AHB_BUS_CTR.BIT.ALIGN_ADDRESS = 0;
    USB10.AHB_BUS_CTR.BIT.MAX_BURST_LEN = 0;
} /* End of function usbh1_ahb_pci_bridge_init() */

/******************************************************************************
Function Name   : usbh1_hid_init
Description     : Host MSC Sample APL Initialize Command Send
Arguments       : none
Return value    : none
******************************************************************************/
void usbh1_hid_init(void)
{
    uint16_t    i,j;

    /* Initialeze hid device state */
    for( i = 1; i < (USBH1_MAXDEVADDR + 1) ; i++ )
    {
        usbh1_hid_dev_info[i].state = STATE_ATTACH;           /* Set Application status  */
        usbh1_hid_dev_info[i].event_cnt = 0;
        for( j = 0; j < USBH1_EVENT_MAX; j++ )
        {
            usbh1_hid_dev_info[i].event[j] = EVENT_NONE;
        }
    }
    
    /* Initialeze hid device state */
    for( i = 0; i < USBH1_MAXDEVADDR ; i++ )
    {
        gs_usb_dev_info[i].interface = USB_IF_NODEVICE;
    }
    
    /* Host application registration */
    usbh1_hid_registration();
    /* Init host class driver task */
    R_USBH1_CstdSetTaskPri(USBH1_HHID_TSK, USBH1_PRI_3); /* Host HID task priority set */
} /* End of function usbh1_hid_init() */


/******************************************************************************
Function Name   : usbh1_hid_registration
Description     : registration of Host msc driver.
Arguments       : none
Return value    : none
******************************************************************************/
void usbh1_hid_registration(void)
{
    st_usbh1_hcdreg_t   driver;
    uint8_t             i;

    /* Driver registration */
    driver.ifclass      = (uint16_t)USBH1_IFCLS_HID;                          /* Use Interface class for MSC. */
    driver.p_tpl        = (uint16_t*)&g_usbh1_hhid_devicetpl;                 /* Target peripheral list. */
    driver.classcheck   = (usbh1_cb_check_t) &R_USBH1_HhidClassCheck;         /* Driver check */
    driver.devconfig    = &usbh1_hid_configured;                              /* Callback when device is configured. */
    driver.devdetach    = &usbh1_hid_detach;                                  /* Callback when device is detached. */
    driver.devsuspend   = &usbh1_hstd_dummy_function;                         /* Callback when device is suspended. */
    driver.devresume    = &usbh1_hid_resume_complete;                         /* Callback when device is resumed. */

    for (i = 0; i < USBH1_MAX_DEVICE; i++)                                    /* Loop support HID device count */
    {
        R_USBH1_HstdDriverRegistration(&driver);                         /* Host HID class driver registration. */
    }
} /* End of function usbh1_hid_registration() */


/******************************************************************************
Function Name   : usbh1_hid_event_set
Description     : Set event.
Arguments       : uint16_t  devadr  : Device Adrress
Return value    : none
******************************************************************************/
void usbh1_hid_event_set(uint16_t devadr, uint16_t event )
{
    if( usbh1_hid_dev_info[devadr].event_cnt < EVENT_MAX )    /* Check max event count */
    {
        usbh1_hid_dev_info[devadr].event[usbh1_hid_dev_info[devadr].event_cnt] = event; /* Set event */
        usbh1_hid_dev_info[devadr].event_cnt++;               /* Update event count */
    }
    if( usbh1_hid_dev_info[devadr].event_cnt >= EVENT_MAX )
    {
        usbh1_hid_dev_info[devadr].event_cnt = 0;
    }
}   /* eof usbh1_hid_event_set() */

/******************************************************************************
Function Name   : usbh1_hid_event_get
Description     : Set event.
Arguments       : uint16_t  devadr  : Device Adrress
Return value    : none
******************************************************************************/
uint16_t usbh1_hid_event_get(uint16_t devadr )
{
    uint16_t    event = EVENT_NONE;
    uint8_t     i;

    if( usbh1_hid_dev_info[devadr].event_cnt > 0 )            /* Check event count */
    {
        event = usbh1_hid_dev_info[devadr].event[0];          /* Get event */
        usbh1_hid_dev_info[devadr].event_cnt--;               /* Update event count */
        for( i = 0; i < usbh1_hid_dev_info[devadr].event_cnt; i++ )    /* Shife event buffer */
        {
            usbh1_hid_dev_info[devadr].event[i] = usbh1_hid_dev_info[devadr].event[(i + 1)];
        }
    }

    return event;
}   /* eof hid_event_get() */


/******************************************************************************
 Function Name   : usbh1_hid_configured
 Description     : Callback function for HID device configuered
 Arguments       : uint16_t     devadr  : Device Adrress
 Return value    : none
 *****************************************************************************/
void usbh1_hid_configured (uint16_t devadr)
{
    usbh1_hid_event_set(devadr, USBH1_STS_CONFIGURED); /* Set Event()  */
    TP_SendEvtMsg(TP_EVTFLG_ATTACH);

} /* End of function usbh1_hid_configured() */
/******************************************************************************
 End of function usbh1_hid_configured
 *****************************************************************************/

/******************************************************************************
Function Name   : usbh1_hid_detach
Description     : Callback function for HID device detach
Arguments       : uint16_t  devadr  : Device Address
Return value    : none
******************************************************************************/
void usbh1_hid_detach(uint16_t devadr)
{
    /* Clear pipe */
    R_USBH1_HstdClearPipe(devadr);
    /* Set Application variables */
    usbh1_hid_dev_info[devadr].state = STATE_ATTACH;
    gs_usb_dev_info[devadr].interface = USB_IF_NODEVICE;
    TP_SendEvtMsg(TP_EVTFLG_DETACH);

} /* End of function usbh1_hid_detach() */

/******************************************************************************
 Function Name   : usbh1_hid_resume_complete
 Description     : Resume complete callback.
 Arguments       : uint16_t     devadr  : Device Adrress
 Return value    : none
 *****************************************************************************/
void usbh1_hid_resume_complete (uint16_t devadr)
{
    usbh1_hid_event_set(devadr, USBH1_STS_RESUME);
}
/******************************************************************************
 End of function usbh1_hid_resume_complete
 *****************************************************************************/

/******************************************************************************
Function Name   : usbh1_hid_class_request_complete
Description     : Host HID class request complete callback.
Arguments       : st_usbh1_utr_t  *utr       : Device address.
Return value    : void
******************************************************************************/
void usbh1_hid_class_request_complete(st_usbh1_utr_t *utr)
{
    if( USBH1_CTRL_END == utr->result )  /* Check usb receive status */
    {
        usbh1_hid_event_set( utr->p_setup->devaddr, USBH1_STS_REQUEST_COMPLETE );   /* Set Event(HID class request complete)  */
    }
}   /* eof usbh1_hid_class_request_complete() */

/******************************************************************************
Function Name   : usbh1_hhid_set_protocol
Description     : Host HID class request SetReport
Arguments       : uint16_t  devadr       : Device address.
                : uint16_t  protocol     : Protocol(0:Boot,1:Report)
                : usbh1_utr_cb_t  complete : Callback function
Return value    : USB_ER_t               : Error info
******************************************************************************/
usbh1_err_t usbh1_hid_set_protocol( uint16_t devadr, uint8_t protocol, usbh1_utr_cb_t complete)
{
    usbh1_err_t    err;

    usbh1_ghhid_class_request_parm.bRequestCode  = USBH1_HID_SET_PROTOCOL; /* Class request setup packet bRequest Set */
    usbh1_ghhid_class_request_parm.set_protocol  = protocol;             /* Protocol(0:Boot,1:Report) */
    usbh1_ghhid_class_request_parm.tranadr       = USBH1_NULL;             /* No-data control */
    usbh1_ghhid_class_request_parm.tranlen       = 0;                    /* data stage transfer size 0 */
    usbh1_ghhid_class_request_parm.complete      = complete;             /* Set callback function */
    usbh1_ghhid_class_request_parm.devadr        = devadr;               /* Device address */

    err = usbh1_hhid_class_request_process();

    return err;
}   /* eof usbh1_hhid_set_protocol() */

/******************************************************************************
Function Name   : usbh1_hhid_class_request_process
Description     : Host HID class request SetReport
Return value    : usbh1_err_t               : Error info
******************************************************************************/
usbh1_err_t usbh1_hhid_class_request_process(void)
{
    usbh1_err_t        err;

    switch(usbh1_ghhid_class_request_parm.bRequestCode)
    {
        case USBH1_HID_SET_REPORT:
            usbh1_shhid_class_request_setup.type   = USBH1_HID_SET_REPORT | USBH1_HOST_TO_DEV | USBH1_CLASS | USBH1_INTERFACE;
            usbh1_shhid_class_request_setup.value  = (uint16_t)USBH1_HID_SET_DT_TYPE((uint16_t)USBH1_HID_DT_TYPE_SETREPORT);
            usbh1_shhid_class_request_setup.index  = (uint16_t)0x0000;
            usbh1_shhid_class_request_setup.length = (uint16_t)usbh1_ghhid_class_request_parm.tranlen;
            break;

        case USBH1_HID_SET_PROTOCOL:
            usbh1_shhid_class_request_setup.type   = USBH1_HID_SET_PROTOCOL | USBH1_HOST_TO_DEV | USBH1_CLASS | USBH1_INTERFACE;
            usbh1_shhid_class_request_setup.value  = (uint16_t)usbh1_ghhid_class_request_parm.set_protocol;
            usbh1_shhid_class_request_setup.index  = (uint16_t)0x0000;
            usbh1_shhid_class_request_setup.length = (uint16_t)0;
            break;

        default:
            break;
    }
    usbh1_shhid_class_request_setup.devadr = usbh1_ghhid_class_request_parm.devadr;
    usbh1_ghhid_class_request_parm.bRequestCode = USBH1_NULL;

    usbh1_shhid_class_control.keyword    = USBH1_PIPE0;
    usbh1_shhid_class_control.p_tranadr  = usbh1_ghhid_class_request_parm.tranadr;
    usbh1_shhid_class_control.tranlen    = usbh1_ghhid_class_request_parm.tranlen;
    usbh1_shhid_class_control.complete   = usbh1_ghhid_class_request_parm.complete;
    usbh1_shhid_class_control.p_setup    = &usbh1_shhid_class_request_setup;

    err = R_USBH1_HstdTransferStart(&usbh1_shhid_class_control);
    if( err == USBH1_ERROR )
    {
        /* error */
        USBH1_PRINTF0( "### usbh1_hhid_class_request_process() : USBH1_ERROR \n");
    }

    return err;
}

/******************************************************************************
 Function Name   : usbh1_data_read
 Description     : USB data read processing
 Arguments       : uint16_t devadr   : Device address.
                 : uint8_t *buf      : Transfer data address
 Return value    : usbh1_er_t        : USB_OK / USB_ERROR.
 *****************************************************************************/
usbh1_er_t usbh1_data_read (uint16_t devadr, uint8_t *buf)
{
    uint8_t pipe;
    uint32_t size;
    usbh1_er_t err;

    pipe   = R_USBH1_HstdGetPipeID(devadr, USBH1_EP_INT, USBH1_EP_IN, 0xFF);
    size   = R_USBH1_HhidGetMxps(pipe);

    usbh1_ghhid_transfer_message[pipe].keyword = pipe; /* Pipe No */
    usbh1_ghhid_transfer_message[pipe].p_tranadr = buf; /* Data address */
    usbh1_ghhid_transfer_message[pipe].tranlen = size; /* Data Size */
    usbh1_ghhid_transfer_message[pipe].complete = &usbh1_hhid_read_complete; /* Callback function */
    err = R_USBH1_HstdTransferStart(&usbh1_ghhid_transfer_message[pipe]);

    return err;
} /* End of function usbh1_data_read() */

/******************************************************************************
 Function Name   : usbh1_hhid_read_complete callback
 Description     : data receive complete process.
 Arguments       : st_usbh1_utr_t    *mess   : Pointer to st_usbh1_utr_t structure.
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return value    : none
 *****************************************************************************/
void usbh1_hhid_read_complete (st_usbh1_utr_t *mess)
{
    uint16_t     devadr;
    uint16_t     status;
    
    devadr = R_USBH1_HstdGetDevAddr(mess->keyword);
    
    switch (mess->result)
    {
        case USBH1_DATA_OK :
        case USBH1_DATA_SHT :
        case USBH1_DATA_OVR :
        case USBH1_DATA_ERR :
            status = 1;
        break;
        default:
            status = 0;
            break;
    }

    if(1 == status)
    {
        usbh1_hid_event_set(devadr, USBH1_STS_READ_COMPLETE);
        status = 0;
    }
    else
    {
        usbh1_hid_event_set(devadr, USBH1_STS_NONE);
    }
}
/******************************************************************************
 End of function usbh1_hhid_read_complete
 *****************************************************************************/

/******************************************************************************
 Function Name   : usbh1_hhid_info_set_touch
 Description     : set USB touch information parameter.
 Arguments       : uint16_t   drvnum   : USB driver number
 Return value    : none
 ******************************************************************************/
static void usbh1_hhid_info_set_touch (uint16_t drvnum)
{
    gs_usb_dev_info[drvnum].interface = USB_IF_TOUCH;
    gs_usb_dev_info[drvnum].copy_data_len = usb_touch_info.buffer_size;
    
    /* set fixed value without using report descriptor */
    gs_usb_dev_info[drvnum].touch = &usb_touch_info;
    
#if ( SELECT_MONITOR == FWXGA_1360_768)
    gs_usb_dev_info[drvnum].touch->dev_resolutionX = 1360;
    gs_usb_dev_info[drvnum].touch->dev_resolutionY = 768;
#elif ( SELECT_MONITOR == XGA_1024_768)
    gs_usb_dev_info[drvnum].touch->dev_resolutionX = 1024;
    gs_usb_dev_info[drvnum].touch->dev_resolutionY = 768;
#elif ( SELECT_MONITOR == SVGA_800_600)
    gs_usb_dev_info[drvnum].touch->dev_resolutionX = 800;
    gs_usb_dev_info[drvnum].touch->dev_resolutionY = 600;
#endif
}
/******************************************************************************
 End of function usbh1_hhid_info_set_touch
 ******************************************************************************/

/******************************************************************************
 Function Name   : usbh1_hhid_info_set_mouse
 Description     : set USB mouse information parameter.
 Arguments       : uint16_t   drvnum   : USB driver number
 Return value    : none
 ******************************************************************************/
static void usbh1_hhid_info_set_mouse (uint16_t drvnum)
{
    gs_usb_dev_info[drvnum].interface = USB_IF_MOUSE;
    gs_usb_dev_info[drvnum].copy_data_len = usb_mouse_info.buffer_size;
    
    /* set fixed value without using report descriptor */
    gs_usb_dev_info[drvnum].mouse = &usb_mouse_info;
}
/******************************************************************************
 End of function usbh1_hhid_info_set_mouse
 ******************************************************************************/
 
/******************************************************************************
 Function Name   : usbh1_hhid_info_set_kerboard
 Description     : set USB keyboard information parameter.
 Arguments       : uint16_t   drvnum   : USB driver number
 Return value    : none
 ******************************************************************************/
static void usbh1_hhid_info_set_keyboard (uint16_t drvnum)
{
    gs_usb_dev_info[drvnum].interface = USB_IF_KEYBOARD;
    gs_usb_dev_info[drvnum].copy_data_len = usb_keyboard_info.buffer_size;
    
    /* set fixed value without using report descriptor */
    gs_usb_dev_info[drvnum].keyboard = &usb_keyboard_info;
}
/******************************************************************************
 End of function usbh1_hhid_info_set_keyboard
 ******************************************************************************/
 
/******************************************************************************
End  Of File
******************************************************************************/
