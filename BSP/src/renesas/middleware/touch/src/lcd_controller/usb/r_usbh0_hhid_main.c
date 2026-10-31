/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usbh0_hhid_main.c
* Description  : USB Host MSC application code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_usb_hhid_apl.h"
#include "r_usb_hhid_control.h"
#include "r_usbh0_hhid_config.h"

#include "r_usbh0_basic_if.h"
#include "r_usbh0_basic_local.h"
#include "r_usbh0_rtos.h"

#include "r_intc_lld_rza2m.h"        /* INTC Driver Header   */
#include "r_os_abstraction_api.h"
#include "driver.h"
#include "iodefine.h"

#include "debug.h"

/***********************************************************************************************************************
Constant macro definitions
***********************************************************************************************************************/
#define USB_KEY_INPUT_BUFF_NUM  (12)
/* ASCII CODE */
#define ASCII_LF                (0x0A)
#define ASCII_CR                (0x0D)
#define ASCII_0                 ('0')
#define ASCII_1                 ('1')

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

uint8_t     usbh0_drive_search_lock;
DEV_INFO_t  usbh0_hid_dev_info[USBH0_MAXDEVADDR];    /* MSC device control structure */

const uint16_t g_usbh0_hhid_devicetpl[] =       /* TPL(Targeted Peripheral List) */
{
    1,                      /* Number of tpl table */
    0,                      /* Reserved */
    USBH0_NOVENDOR,         /* Vendor ID  : no-check */
    USBH0_NOPRODUCT,        /* Product ID : no-check */
};

uint16_t        g_usbh0_msg_check;

static usbh0_setup_t    usbh0_shhid_class_request_setup  __attribute__ ((section ("USB_CLASS_0")));                   /* Control Transfer Request field */
static st_usbh0_utr_t   usbh0_shhid_class_control  __attribute__ ((section ("USB_CLASS_0")));                         /* Request for HID to USB Request */
static st_usbh0_utr_t   usbh0_shhid_data_control  __attribute__ ((section ("USB_CLASS_0")));                          /* Request for HID to USB Request */
static st_usbh0_utr_t   usbh0_ghhid_transfer_message[USBH0_MAXPIPE]  __attribute__ ((section ("USB_CLASS_0")));
USBH0_HHID_CLASS_REQUEST_PARM_t   usbh0_ghhid_class_request_parm  __attribute__ ((section ("USB_CLASS_0")));          /* Host HID Class Request Parametor */
static uint8_t           g_usbh0_buf[USB_BUF_LEN] __attribute__ ((section ("USB_CLASS_0")));

void          usbh0_hid_class_request_complete(st_usbh0_utr_t *utr);
usbh0_er_t    usbh0_data_read (uint16_t devadr, uint8_t *buf);
usbh0_err_t   usbh0_hid_set_protocol( uint16_t devadr, uint8_t protocol, usbh0_utr_cb_t complete);
void          usbh0_hid_registration(void);
void          usbh0_hhid_read_complete (st_usbh0_utr_t *mess);
void          usbh0_hid_event_set(uint16_t devadr, uint16_t event );
uint16_t      usbh0_hid_event_get(uint16_t devadr);

/******************************************************************************
Function Name   : usbh0_hid_main
Description     : Host MSC application main process
Arguments       : none
Return value    : none
******************************************************************************/
void usbh0_hid_main(void)
{
        uint16_t    ret_val;
        uint16_t    drvno;
        uint16_t    devadr;
        uint16_t    event;
        uint16_t    drvnum;
        uint8_t     device;

    for(drvnum = 0; drvnum < USBH0_MAXDEVADDR; drvnum++)
    {
            event = usbh0_hid_event_get(drvnum);
            /* Check application state */
            switch (event)
            {
            case USBH0_STS_CONFIGURED :
                device = usbh0_hhid_get_hid_protocol(drvnum);
                if ((USBH0_HID_KEYBOARD == device) || (USBH0_HID_MOUSE == device))
                {
                    usbh0_hid_set_protocol( drvnum, BOOT_PROTOCOL, &usbh0_hid_class_request_complete );
                }
                else
                {
                    usbh0_data_read (drvnum, &g_usbh0_buf);
                }
            break;

            case USBH0_STS_READ_COMPLETE :
                usbh0_data_read (drvnum, &g_usbh0_buf);
            break;

            case USBH0_STS_REQUEST_COMPLETE :
                usbh0_data_read (drvnum, &g_usbh0_buf);
            break;

            default :
            break;
            }
        }

}   /* End of function usbh0_hid_main() */

/***********************************************************************************************************************
Function Name   : usbh0_cstd_PortInit
Description     : Port Initialize
Argument        : none
Return          : none
***********************************************************************************************************************/
void usbh0_cstd_PortInit(void)
{
    /* SW3 : IRQ6 */
    PORTJ.PDR.BIT.PDR1 = 0x0;
    PORTJ.PMR.BIT.PMR1 = 0x0;
//    GPIO.PJ1PFS.BIT.PSEL = 0x06;   /* ISEL = b'0, PSEL = b'110 */

#ifdef A2M_NEW_BOARD
    /* VBUSEN1 : PC6 *//* OVRCUR0 : PC7 */
    /* ID : PL2 *//* INT_N : PL3 */
    /* ---- PFS Register Write Protect : OFF  ---- */
    GPIO.PWPR.BIT.B0WI = 0;
    GPIO.PWPR.BIT.PFSWE = 1;

    GPIO.PC6PFS.BIT.PSEL = 0x01;   /* ISEL = b'0, PSEL = b'001 */
    GPIO.PC7PFS.BIT.PSEL = 0x01;   /* ISEL = b'0, PSEL = b'001 */

    /* ---- PFS Register Write Protect : ON ---- */
    GPIO.PWPR.BIT.PFSWE = 0x0;
    GPIO.PWPR.BIT.B0WI = 0x1;

    PORTC.PMR.BIT.PMR6 = 0x1;
    PORTC.PMR.BIT.PMR7 = 0x1;
#else
    PORTC.PDR.BIT.PDR6 = 0x3;
    PORTC.PMR.BIT.PMR6 = 0x0;
    PORTC.PODR.BIT.PODR6 = 0x1;

    /* VBUSEN1 : PC6 *//* OVRCUR0 : PC7 */
    /* ID : PL2 *//* INT_N : PL3 */
    /* ---- PFS Register Write Protect : OFF  ---- */
    GPIO.PWPR.BIT.B0WI = 0;
    GPIO.PWPR.BIT.PFSWE = 1;

    GPIO.PC7PFS.BIT.PSEL = 0x01;   /* ISEL = b'0, PSEL = b'001 */

    /* ---- PFS Register Write Protect : ON ---- */
    GPIO.PWPR.BIT.PFSWE = 0x0;
    GPIO.PWPR.BIT.B0WI = 0x1;

    PORTC.PMR.BIT.PMR7 = 0x1;
#endif
} /* End of function usbh0_cstd_PortInit() */

/***********************************************************************************************************************
* Function Name: usbh0_ahb_pci_bridge_init
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void usbh0_ahb_pci_bridge_init(void)
{
    USB00.INT_ENABLE.BIT.WAKEON_INTEN   = 1;
    USB00.INT_ENABLE.BIT.UCOM_INTEN     = 1;
    USB00.INT_ENABLE.BIT.USBH_INTBEN    = 1;
    USB00.INT_ENABLE.BIT.USBH_INTAEN    = 1;
    USB00.INT_ENABLE.BIT.AHB_INTEN      = 1;

    USB00.AHB_BUS_CTR.BIT.PROT_TYPE     = 0;
    USB00.AHB_BUS_CTR.BIT.PROT_MODE     = 0;
    USB00.AHB_BUS_CTR.BIT.ALIGN_ADDRESS = 0;
    USB00.AHB_BUS_CTR.BIT.MAX_BURST_LEN = 0;
} /* End of function usbh0_ahb_pci_bridge_init() */

/***********************************************************************************************************************
Function Name   : usbh0_hid_init
Description     : Host MSC Sample APL Initialize Command Send
Arguments       : none
Return value    : none
***********************************************************************************************************************/
void usbh0_hid_init(void)
{
    uint16_t    i,j;

    /* Initialeze hid device state */
    for( i = 1; i < (USBH0_MAXDEVADDR + 1) ; i++ )
    {
        usbh0_hid_dev_info[i].state = STATE_ATTACH;           /* Set Application status  */
        usbh0_hid_dev_info[i].event_cnt = 0;
        for( j = 0; j < USBH0_EVENT_MAX; j++ )
        {
            usbh0_hid_dev_info[i].event[j] = EVENT_NONE;
        }
    }
    /* Host application registration */
    usbh0_hid_registration();
    /* Init host class driver task */
    R_USBH0_CstdSetTaskPri(USBH0_HHID_TSK, USBH0_PRI_3); /* Host HID task priority set */
} /* End of function usbh0_hid_init() */


/***********************************************************************************************************************
Function Name   : usbh0_hid_registration
Description     : registration of Host msc driver.
Arguments       : none
Return value    : none
***********************************************************************************************************************/
void usbh0_hid_registration(void)
{
    st_usbh0_hcdreg_t   driver;
    uint8_t             i;

    /* Driver registration */
    driver.ifclass      = (uint16_t)USBH0_IFCLS_HID;                          /* Use Interface class for MSC. */
    driver.p_tpl        = (uint16_t*)&g_usbh0_hhid_devicetpl;                 /* Target peripheral list. */
    driver.classcheck   = (usbh0_cb_check_t) &R_USBH0_HhidClassCheck;         /* Driver check */
    driver.devconfig    = &usbh0_hid_configured;                              /* Callback when device is configured. */
    driver.devdetach    = &usbh0_hid_detach;                                  /* Callback when device is detached. */
    driver.devsuspend   = &usbh0_hstd_dummy_function;                         /* Callback when device is suspended. */
    driver.devresume    = &usbh0_hid_resume_complete;                         /* Callback when device is resumed. */

    for (i = 0; i < USBH0_MAX_DEVICE; i++)                                    /* Loop support HID device count */
    {
        R_USBH0_HstdDriverRegistration(&driver);                         /* Host HID class driver registration. */
    }
} /* End of function usbh0_hid_registration() */


/******************************************************************************
Function Name   : usbh0_hid_event_set
Description     : Set event.
Arguments       : uint16_t  devadr  : Device Adrress
Return value    : none
******************************************************************************/
void usbh0_hid_event_set(uint16_t devadr, uint16_t event )
{
    if( usbh0_hid_dev_info[devadr].event_cnt < EVENT_MAX )    /* Check max event count */
    {
        usbh0_hid_dev_info[devadr].event[usbh0_hid_dev_info[devadr].event_cnt] = event; /* Set event */
        usbh0_hid_dev_info[devadr].event_cnt++;               /* Update event count */
    }
    if( usbh0_hid_dev_info[devadr].event_cnt >= EVENT_MAX )
    {
        usbh0_hid_dev_info[devadr].event_cnt = 0;
    }
}   /* eof usbh0_hid_event_set() */

/******************************************************************************
Function Name   : usbh0_hid_event_get
Description     : Set event.
Arguments       : uint16_t  devadr  : Device Adrress
Return value    : none
******************************************************************************/
uint16_t usbh0_hid_event_get(uint16_t devadr )
{
    uint16_t    event = EVENT_NONE;
    uint8_t     i;

    if( usbh0_hid_dev_info[devadr].event_cnt > 0 )            /* Check event count */
    {
        event = usbh0_hid_dev_info[devadr].event[0];          /* Get event */
        usbh0_hid_dev_info[devadr].event_cnt--;               /* Update event count */
        for( i = 0; i < usbh0_hid_dev_info[devadr].event_cnt; i++ )    /* Shife event buffer */
        {
            usbh0_hid_dev_info[devadr].event[i] = usbh0_hid_dev_info[devadr].event[(i + 1)];
        }
    }

    return event;
}   /* eof hid_event_get() */


/******************************************************************************
 Function Name   : usbh0_hid_configured
 Description     : Callback function for HID device configuered
 Arguments       : uint16_t     devadr  : Device Adrress
 Return value    : none
 ******************************************************************************/
void usbh0_hid_configured (uint16_t devadr)
{
    usbh0_hid_event_set(devadr, USBH0_STS_CONFIGURED); /* Set Event()  */
}
/******************************************************************************
 End of function usbh0_hid_configured
 ******************************************************************************/


/***********************************************************************************************************************
Function Name   : usbh0_hid_detach
Description     : Callback function for HID device detach
Arguments       : uint16_t  devadr  : Device Address
Return value    : none
***********************************************************************************************************************/
void usbh0_hid_detach(uint16_t devadr)
{
    /* Clear pipe */
    R_USBH0_HstdClearPipe(devadr);
    /* Set Application variables */
    usbh0_hid_dev_info[devadr].state = STATE_ATTACH;
} /* End of function usbh0_hid_detach() */

/******************************************************************************
 Function Name   : usbh0_hid_resume_complete
 Description     : Resume complete callback.
 Arguments       : uint16_t     devadr  : Device Adrress
 Return value    : none
 ******************************************************************************/
void usbh0_hid_resume_complete (uint16_t devadr)
{
    usbh0_hid_event_set(devadr, USBH0_STS_RESUME);
}
/******************************************************************************
 End of function usbh0_hid_resume_complete
 ******************************************************************************/

/******************************************************************************
Function Name   : usbh0_hid_class_request_complete
Description     : Host HID class request complete callback.
Arguments       : st_usbh0_utr_t  *utr       : Device address.
Return value    : void
******************************************************************************/
void usbh0_hid_class_request_complete(st_usbh0_utr_t *utr)
{
    if( USBH0_CTRL_END == utr->result )  /* Check usb receive status */
    {
        usbh0_hid_event_set( utr->p_setup->devaddr, USBH0_STS_REQUEST_COMPLETE );   /* Set Event(HID class request complete)  */
    }
}   /* eof usbh0_hid_class_request_complete() */

/******************************************************************************
Function Name   : usbh0_hhid_set_protocol
Description     : Host HID class request SetReport
Arguments       : uint16_t  devadr       : Device address.
                : uint16_t  protocol     : Protocol(0:Boot,1:Report)
                : usbh0_utr_cb_t  complete : Callback function
Return value    : USB_ER_t               : Error info
******************************************************************************/
usbh0_err_t usbh0_hid_set_protocol( uint16_t devadr, uint8_t protocol, usbh0_utr_cb_t complete)
{
    usbh0_err_t    err;

    usbh0_ghhid_class_request_parm.bRequestCode  = USBH0_HID_SET_PROTOCOL; /* Class request setup packet bRequest Set */
    usbh0_ghhid_class_request_parm.set_protocol  = protocol;             /* Protocol(0:Boot,1:Report) */
    usbh0_ghhid_class_request_parm.tranadr       = USBH0_NULL;             /* No-data control */
    usbh0_ghhid_class_request_parm.tranlen       = 0;                    /* data stage transfer size 0 */
    usbh0_ghhid_class_request_parm.complete      = complete;             /* Set callback function */
    usbh0_ghhid_class_request_parm.devadr        = devadr;               /* Device address */

    err = usbh0_hhid_class_request_process();

    return err;
}   /* eof usbh0_hhid_set_protocol() */

/******************************************************************************
Function Name   : usbh0_hhid_class_request_process
Description     : Host HID class request SetReport
Return value    : usbh0_err_t               : Error info
******************************************************************************/
usbh0_err_t usbh0_hhid_class_request_process(void)
{
    usbh0_err_t        err;

    switch(usbh0_ghhid_class_request_parm.bRequestCode)
    {
        case USBH0_HID_SET_REPORT:
            usbh0_shhid_class_request_setup.type   = USBH0_HID_SET_REPORT | USBH0_HOST_TO_DEV | USBH0_CLASS | USBH0_INTERFACE;
            usbh0_shhid_class_request_setup.value  = (uint16_t)USBH0_HID_SET_DT_TYPE((uint16_t)USBH0_HID_DT_TYPE_SETREPORT);
            usbh0_shhid_class_request_setup.index  = (uint16_t)0x0000;
            usbh0_shhid_class_request_setup.length = (uint16_t)usbh0_ghhid_class_request_parm.tranlen;
            break;

        case USBH0_HID_SET_PROTOCOL:
            usbh0_shhid_class_request_setup.type   = USBH0_HID_SET_PROTOCOL | USBH0_HOST_TO_DEV | USBH0_CLASS | USBH0_INTERFACE;
            usbh0_shhid_class_request_setup.value  = (uint16_t)usbh0_ghhid_class_request_parm.set_protocol;
            usbh0_shhid_class_request_setup.index  = (uint16_t)0x0000;
            usbh0_shhid_class_request_setup.length = (uint16_t)0;
            break;

        default:
            break;
    }
    usbh0_shhid_class_request_setup.devadr = usbh0_ghhid_class_request_parm.devadr;
    usbh0_ghhid_class_request_parm.bRequestCode = USBH0_NULL;

    usbh0_shhid_class_control.keyword  = USBH0_PIPE0;
    usbh0_shhid_class_control.p_tranadr  = usbh0_ghhid_class_request_parm.tranadr;
    usbh0_shhid_class_control.tranlen  = usbh0_ghhid_class_request_parm.tranlen;
    usbh0_shhid_class_control.complete = usbh0_ghhid_class_request_parm.complete;
    usbh0_shhid_class_control.p_setup    = &usbh0_shhid_class_request_setup;

    err = R_USBH0_HstdTransferStart(&usbh0_shhid_class_control);
    if( err == USBH0_ERROR )
    {
        /* error */
        USBH0_PRINTF0( "### usbh0_hhid_class_request_process() : USBH0_ERROR \n");
    }

    return err;
}

/******************************************************************************
 Function Name   : usbh0_data_read
 Description     : USB data read processing
 Arguments       : uint16_t devadr   : Device address.
                 : uint8_t *buf      : Transfer data address
 Return value    : usbh0_er_t        : USB_OK / USB_ERROR.
 ******************************************************************************/
usbh0_er_t usbh0_data_read (uint16_t devadr, uint8_t *buf)
{
    uint8_t pipe;
    uint32_t size;
    usbh0_er_t err;

    pipe   = R_USBH0_HstdGetPipeID(devadr, USBH0_EP_INT, USBH0_EP_IN, 0xFF);
    size   = R_USBH0_HhidGetMxps(pipe);

    usbh0_ghhid_transfer_message[pipe].keyword = pipe; /* Pipe No */
    usbh0_ghhid_transfer_message[pipe].p_tranadr = buf; /* Data address */
    usbh0_ghhid_transfer_message[pipe].tranlen = size; /* Data Size */
    usbh0_ghhid_transfer_message[pipe].complete = &usbh0_hhid_read_complete; /* Callback function */
    err = R_USBH0_HstdTransferStart(&usbh0_ghhid_transfer_message[pipe]);

    return err;
} /* End of function usbh0_data_read() */

/******************************************************************************
 Function Name   : usbh0_hhid_read_complete callback
 Description     : data receive complete process.
 Arguments       : st_usbh0_utr_t    *mess   : Pointer to st_usbh0_utr_t structure.
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
void usbh0_hhid_read_complete (st_usbh0_utr_t *mess)
{
    uint16_t     devadr;
    uint16_t     status;

    devadr = R_USBH0_HstdGetDevAddr(mess->keyword);

    switch (mess->result)
    {
        case USBH0_DATA_OK :
        case USBH0_DATA_SHT :
        case USBH0_DATA_OVR :
        case USBH0_DATA_ERR :
            status = 1;
        break;
        default:
            status = 0;
            break;
    }

    if(1 == status)
    {
        usbh0_hid_event_set(devadr, USBH0_STS_READ_COMPLETE);
        status = 0;
    }
    else
    {
        usbh0_hid_event_set(devadr, USBH0_STS_NONE);
    }
}
/******************************************************************************
 End of function usbh0_hhid_read_complete
 ******************************************************************************/

/***********************************************************************************************************************
End  Of File
***********************************************************************************************************************/
