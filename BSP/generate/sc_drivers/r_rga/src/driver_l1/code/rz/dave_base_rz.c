//--------------------------------------------------------------------------
// Project: D/AVE
// File:    dave_base_nios.c (%version: 8 %)
//          created Fri Aug 19 15:32:32 2005 by hh04027
//
// Description:
//  %date_modified: Thu Oct 18 13:18:03 2007 %  (%derived_by:  hh74040 %)
//
// Changes:
//  2005-04-12 CSe  fully implemented missing d1 driver functionality
//  2006-10-18 ASc  add ifdefs to allow compilation with no altera lcd
//  2009-03-17 MMa  added support for TES Display Controller
//  2011-08-03 CSe  additional ifdefs in case single SOPC components not used
//  2011-10-04 CSe  removed references to BT656 generator (component no longer used)
//  2018-01-23 Changed to for RZ/A2M
//  $ModuleRevision: {Masters}\DRW\D_AVE_2D\RZA2\Port\010_19_en $
//
//--------------------------------------------------------------------------

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "dave_base.h"
#include "dave_base_rz.h"
#include "use_config_drw.h"
#ifdef  USE_LIST_RZA2M
#include "r_drw_pl.h"
#endif


//--------------------------------------------------------------------------
//
d1_device * d1_opendevice(long flags)
{
    d1_device_rz*     handle;
    volatile uint8_t  dummy_buf;

    /* unused arguments */
    INTERNAL_NOT_USED(flags);

    /* get new device context structure */
    handle = (d1_device_rz *)malloc(sizeof(d1_device_rz));
    if (!handle)
    {
        return NULL;
    }

    /* init device data */
    handle->dlist_indirect = 0;

    /* Starts to supply DAVE(=DRW) clock. */
    /* Registers DRW interrupt callback funcion calling <R_DRW_OnInterrupting> function. */
    /* Sets DRW interrupt priority. */
    #ifdef  USE_LIST_RZA2M
        R_DRW_OnInitialize();
    #endif

    /* reset irq handlers */
    d1_initirq_intern(handle);

    return (d1_device *)handle;
}

//--------------------------------------------------------------------------
//
int d1_closedevice(d1_device *handle)
{
    int               ret = 0;

    if (!handle)
    {
        return 0;
    }

    /* close irq handlers */
    ret = d1_shutdownirq_intern((d1_device_rz *) handle);

    /* power down peripheral */
    #ifdef  USE_LIST_RZA2M
        R_DRW_OnFinalize();
    #endif

    free (handle);
    return ret;
}

//--------------------------------------------------------------------------
//
void d1_setregister(d1_device *handle, int deviceid, int index, long value)
{
    d1_device_rz *dev = (d1_device_rz*) handle;

    switch (deviceid)
    {
    case D1_DAVE2D:
        if ((index == D2_DLISTSTART) && (dev->dlist_indirect))
        {
            int *dlistp = (int*)value;
            dev->dlist_start = (long*)(dlistp + 1);
            WRITE_REG(DAVE2D_0_BASE, index, (unsigned long)(*dlistp));
        }
        else
        {
            WRITE_REG(DAVE2D_0_BASE, index, (unsigned long)value);
        }
    break;

    case D1_DLISTINDIRECT:
#if RX_USE_DLIST_INDIRECT
        dev->dlist_indirect = value;
#else
        dev->dlist_indirect = 0;
#endif
    break;

    default:
        
        /* unknown device */
    break;
    }
}

//--------------------------------------------------------------------------
//
long d1_getregister(d1_device *handle, int deviceid, int index)
{
    d1_device_rz *dev = (d1_device_rz*)handle;

    switch (deviceid)
    {
    case D1_DAVE2D:
        return READ_REG(DAVE2D_0_BASE, index);

    case D1_DLISTINDIRECT:
        return dev->dlist_indirect;

    default:

        /* unknown device */
        return 0;
    }
}

//--------------------------------------------------------------------------
//
int d1_devicesupported(d1_device *handle, int deviceid)
{
    /* unused arguments */
    INTERNAL_NOT_USED(handle);

    switch (deviceid)
    {
    case D1_DAVE2D:
#if RX_USE_DLIST_INDIRECT
    case D1_DLISTINDIRECT:
#endif
        return 1;

    default:

        /* unknown device */
        return 0;
    }
}

