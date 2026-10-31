/*******************************************************************************
* @file         main.c
* @brief        Main Entry for RZ/A2M LVGL Application
******************************************************************************/

#include "hal/rza2m_hal.h"

int main(void)
{
    /* Hardware BSP initialization & FreeRTOS kernel startup */
    rza2m_hal_init();

    while(1);
}
