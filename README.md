# LVGL ported to Renesas EK-RZ/A2M
🚀 This repository is available in the LVGL Project Creator, making it easy to create and customize a new project in just a few clicks.

## Overview
The Renesas EK-RZ/A2M board is an evaluation kit for the RZ/A2M MPU. It is an MPU but it has a focus on baremetal and RTOS applications. It boasts a mighty 1 GHz Cortex-A55 CPU and 128 MB of DDR3L-SDRAM. It is easy to use the low-level IO, of which it provides a lot. The board offers connectivity to many ecosystems like Arduino, Qwiic, Grove, mikroBUS, and Pmod. A 1280x720 display is included in the kit. It mounts to the board via a board-to-board connector.

## Buy
The Renesas EK-RZ/A2M can be purchased from https://www.renesas.com/en/design-resources/boards-kits/rz-a2m-evkit

## Modify the Project
Open a demo
The entry point is contained in src/LVGL_thread_entry.c.

You can disable the LVGL demos (lv_demo_benchmark()) (or just comment them out) and call some lv_example_...() functions, or add your own custom code.

## Configuration
Edit lv_conf.h to configure LVGL.

It will automatically run any demo that is enabled in lv_conf.h. You can see lv_conf.defaults for a summary of the configs which have been changed from the defaults.
