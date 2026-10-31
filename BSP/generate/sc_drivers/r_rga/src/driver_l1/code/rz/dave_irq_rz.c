//--------------------------------------------------------------------------
// Project: D/AVE
// File:    dave_irq_nios.c (%version: 8 %)
//          created Tue Aug 23 09:26:54 2005 by hh04027
//
// Description:
//  %date_modified: Thu Oct 18 13:28:42 2007 %  (%derived_by:  hh74040 %)
//
// Changes:
//  2006-01-04 CSe  adapted to changes in dave_base.h
//  2005-04-12 CSe  fully implemented missing d1 driver functionality
//  2007-10-18 ASc  added ifdefs to allow compilation without altera lcd
//  2009-03-17 MMa  added support for TES Display Controller
//  2012-08-03 CSe  clearing interrupt before calling registered IRQ handler
//  2018-01-23 Changed to for RZ/A2M
//  $ModuleRevision: {Masters}\DRW\D_AVE_2D\RZA2\Port\010_19_jp $
//--------------------------------------------------------------------------

#include "use_config_drw.h"
#include "use_config_rga.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "dave_base.h"
#include "dave_base_rz.h"
#ifdef  USE_LIST_RZA2M
#include "r_drw_pl.h"
#include "rz_co.h"
#endif
#include "dave_driver.h"
#ifdef  USE_CONFIG_DRW_INTERRUPT_LOG
#include "r_debug.h"
#include "rz_co.h"
#endif

/* Register */
#define DAVE_STAT        0x0
#define DAVE_IRQ_CTRL    0x30  /* D2_IRQCTL in "dave_registermap.h" */

/* Bit mask */
#define DRW_IRQCTL_DLISTIRQCLR  0x00000008u
#define DRW_IRQCTL_ENUMIRQCLR   0x00000004u
#define DRW_IRQCTL_DLISTIRQEN   0x00000002u

/* Bit shift */
#define DRW_IRQCTL_DLISTIRQCLR_SHIFT  3
#define DRW_IRQCTL_ENUMIRQCLR_SHIFT   2
#define DRW_IRQCTL_DLISTIRQEN_SHIFT   1


//--------------------------------------------------------------------------

enum d1_irqslots
{
	d1_irqslot_break = 0, 
	d1_irqslot_vbi = 1, 
	d1_irqslot_dlist = 2, 
	d1_irqcount
};

//--------------------------------------------------------------------------

static d1_interrupt g_irq_handler[d1_irqcount];
static void *g_irq_data[d1_irqcount];
static volatile int g_irq_triggered[d1_irqcount];
static d1_device_rz*  g_context;


//--------------------------------------------------------------------------
// internal
static int d1_mapirq_intern(int irqtype)
{
	int index;

	// map irqtype bit to slot index
	switch (irqtype)
	{
		case d1_irq_break:
			index = d1_irqslot_break;
			break;

		case d1_irq_vbi:
			index = d1_irqslot_vbi;
			break;

		case d1_irq_dlist:
			index = d1_irqslot_dlist;
			break;

#ifdef __ALTERA_AVALON_BT656_VIDEOIN
		case d1_irq_vip:
			index = d1_irqslot_vip;
			break;
#endif

		default:
			index = -1;
	}

	return index;
}


//--------------------------------------------------------------------------
//
#ifdef  USE_LIST_RZA2M
void  R_DRW_OnInterrupting(void)
#else
#error
#endif
{
	uint32_t  intReg;
	d1_device_rz *dev = (d1_device_rz*) g_context;
	#ifdef  USE_CONFIG_DRW_INTERRUPT_LOG
		R_D_AddToIntLog( 0x505 );  /* start "d1_dave2d_isr" */
	#endif

	intReg = READ_REG(DAVE2D_0_BASE, DAVE_STAT);
	if (intReg & 0x30)
	{  // any DAVE interrupt?
		#ifdef  USE_CONFIG_DRW_INTERRUPT_LOG
			R_D_AddToIntLog( 0x506 );  /* got "DAVE_STAT" and clear interrupt bits */
			R_D_AddToIntLog( intReg );
		#endif
		// clear/enable all DAVE interrupts
		WRITE_REG(DAVE2D_0_BASE, DAVE_IRQ_CTRL, 0xE);
			/*
			* 0xE:
			*    DRW_IRQCTL_DLISTIRQCLR |
			*    DRW_IRQCTL_ENUMIRQCLR |
			*    DRW_IRQCTL_DLISTIRQEN;
			*/
		
		if (intReg & 0x20)
		{ // display list interrupt?
			if ( dev->dlist_indirect && *(dev->dlist_start) != 0)
			{
				long dlist_addr;
				dlist_addr = *(dev->dlist_start);
				/* get next dlist start address */
				dev->dlist_start++;
				/* starting Dave */
				WRITE_REG( DAVE2D_0_BASE, D2_DLISTSTART, dlist_addr );
			}
			else 
			{
				if (g_irq_handler[d1_irqslot_dlist])
					g_irq_handler[d1_irqslot_dlist](d1_irq_dlist, 0, g_irq_data[d1_irqslot_dlist]);

				g_irq_triggered[d1_irqslot_dlist] = 1;
			}
		}
		// while (READ_REG(DAVE2D_0_BASE, DAVE_STAT) & 0x30);

		R_DRW_SetInterruptEvent();
	}
}


//--------------------------------------------------------------------------
// initializaton + deinitialization
int d1_initirq_intern(d1_device_rz *handle)
{
	int i;
	int error = 0;

#ifdef  USE_LIST_RZA2M
	errnum_t  e;
#endif

	for (i=0; i<d1_irqcount; ++i) {
		g_irq_handler[i]   = NULL;
		g_irq_data[i]      = NULL;
		g_irq_triggered[i] = 0;
	}    

	/* In <d1_opendevice> */
	/* Starts to supply DAVE(=DRW) clock. */
	/* Registers DRW interrupt callback funcion calling <R_DRW_OnInterrupting> function. */
	/* Sets DRW interrupt priority. */

	#ifdef  USE_LIST_RZA2M
		g_context = handle;
	#else
		error = 1;
		goto fin;
	#endif

	/* clear DAVE irq's, enable dlist irq */
	WRITE_REG(DAVE2D_0_BASE, DAVE_IRQ_CTRL, 0xe);
		/*
		* 0xe:
		*    DRW_IRQCTL_DLISTIRQCLR |
		*    DRW_IRQCTL_ENUMIRQCLR |
		*    DRW_IRQCTL_DLISTIRQEN;
		*/

	/* enable DAVE irq */
	#ifdef  USE_LIST_RZA2M
		R_DRW_EnableInterrupt();
	#else
		error = 1;
		goto fin;
	#endif

	error = 0;
fin:
	if ( error ) {
		d1_shutdownirq_intern( handle );
	}
	return !error;
}

int d1_shutdownirq_intern(d1_device_rz *handle)
{
	int error = 0;

	uint32_t  value;
#ifdef  USE_LIST_RZA2M
	errnum_t  e;
#endif
	volatile uint32_t  dummy;

	/* unused arguments */
	(void) handle;

	/* disable DAVE irq */
	#ifdef  USE_LIST_RZA2M
		R_DRW_DisableInterrupt();
	#else
		error |= 1;
	#endif

fin:
	return !error;
}


//--------------------------------------------------------------------------
//
void d1_setirqhandler(d1_device *handle, int irqtype, d1_interrupt code, void *data)
{
	int irqslot;

	// unused arguments
	(void) handle;

	// get slot
	irqslot = d1_mapirq_intern( irqtype );
	if (irqslot<0) return;

	// store irq handler and pointer
	g_irq_handler[ irqslot ] = code;
	g_irq_data[ irqslot ] = data;
}


//--------------------------------------------------------------------------
//
d1_interrupt d1_getirqhandler(d1_device *handle, int irqtype)
{
	int irqslot;

	// unused arguments
	(void) handle;

	// get slot
	irqslot = d1_mapirq_intern( irqtype );
	if (irqslot<0) return NULL;

	return g_irq_handler[ irqslot ];
}


//--------------------------------------------------------------------------
//
void * d1_getirqdata(d1_device *handle, int irqtype)
{
	int irqslot;

	// unused arguments
	(void) handle;

	// get slot
	irqslot = d1_mapirq_intern( irqtype );
	if (irqslot<0) return NULL;

	return g_irq_data[ irqslot ];
}


//--------------------------------------------------------------------------
//
int d1_queryirq(d1_device *handle, int irqmask, int timeout)
{
	// unused arguments
	(void) handle;

	/* break + vip + vbi interrupts are not supported */
	if (irqmask & ~d1_irq_break & ~d1_irq_vip & ~d1_irq_vbi)
	{
		if ((irqmask & d1_irq_dlist) && g_irq_triggered[d1_irqslot_dlist])
		{
			g_irq_triggered[d1_irqslot_dlist] = 0;
			return d1_irq_dlist;
		}

		if (timeout == d1_to_no_wait)
		{
			return 0;
		}

		/* wait for irq */
		while(1)
		{
			#ifdef  USE_LIST_RZA2M
			{
				errnum_t  e;

				e= R_DRW_WaitForInterruptEvent();
				if(e){
					R_ERROR_Set( e );
					return 1;
				}
			}
			#endif

			if ((irqmask & d1_irq_dlist) && g_irq_triggered[d1_irqslot_dlist])
			{
				g_irq_triggered[d1_irqslot_dlist] = 0;
#ifdef  USE_CONFIG_DRW_INTERRUPT_LOG
				R_D_AddToIntLog( 0x519 );  /* Exit waiting */
#endif
				return d1_irq_dlist;
			}
#ifdef  USE_CONFIG_DRW_INTERRUPT_LOG
			else {
				static int  interval_count = 0;
				static int  interval_count_max = 100000;

				interval_count += 1;
				if ( interval_count >= interval_count_max ) {
					int  value_of_STAT = READ_REG( DAVE2D_0_BASE,  DAVE_STAT );
					R_D_AddToIntLog( 0x518 );  /* Check status */
					R_D_AddToIntLog( value_of_STAT );
				}
			}
#endif
		}
	}

	return 0;
}


//--------------------------------------------------------------------------
//
int d1_callirqhandler(d1_device *handle, int irqtype, void *irqdata)
{
	int irqslot;

	// unused arguments
	(void) handle;

	// get slot
	irqslot = d1_mapirq_intern( irqtype );
	if (irqslot<0) return 0;

	// call irq handler
	if (!g_irq_handler[ irqslot ]) return 1;
	g_irq_handler[ irqslot]( irqtype, irqdata, g_irq_data[ irqslot ] );

	return 1;
}


