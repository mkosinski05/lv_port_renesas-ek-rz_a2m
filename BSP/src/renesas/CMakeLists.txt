# FreeRTOS for Renesas RZ/A2M Boards
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(RZA2M_Renesas)
SET(PROJECT_VERSION 10.0.0)

SET(RENESAS_INC
  "${PROJECT_SOURCE_DIR}/application/common/camera/camera_mipi_imx219_b_regdata.h"
  "${PROJECT_SOURCE_DIR}/application/inc/application_cfg.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd_panel.h"
  "${PROJECT_SOURCE_DIR}/application/inc/mipi_camera.h"
  "${PROJECT_SOURCE_DIR}/application/inc/os_port_freertos.h"
  "${PROJECT_SOURCE_DIR}/application/inc/perform.h"
  "${PROJECT_SOURCE_DIR}/application/inc/r_display_init.h"
  "${PROJECT_SOURCE_DIR}/application/inc/r_image_config.h"
  "${PROJECT_SOURCE_DIR}/application/inc/r_scifa_rza2_config.h"
  "${PROJECT_SOURCE_DIR}/application/inc/r_task_priority.h"
  "${PROJECT_SOURCE_DIR}/application/inc/version.h"
  "${PROJECT_SOURCE_DIR}/application/inc/camera/camera_imx219_b.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/lcd_dvi_ch0.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/lcd_dvi_clk.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/lcd_lvds_ch0.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/lcd_lvds_clk.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/lcd_rsk_ch0.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/lcd_rsk_clk.h"
  "${PROJECT_SOURCE_DIR}/application/inc/lcd/pc_monitor.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/inc/dev_drv.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/inc/lcd_controller_if.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/inc/tp_if.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/FT5216/lcd_ft5216.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/FT5216/lcd_ft5216_int.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/inc/r_usb_hhid_apl.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/inc/r_usb_gui_control.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/inc/r_usb_hhid_control.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/inc/r_usb_hhid_dev.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/usb_gui_data/arrow_data.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/usb_gui_data/caution_data.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_task.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_task_riic.h"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_task_usb.h"
)

SET(HEADERS_LIST_RENESAS
	${RENESAS_INC}
	PARENT_SCOPE
)

SOURCE_GROUP("Header Files\\Renesas" FILES ${RENESAS_INC})

SET(RENESAS_SRC
  "${PROJECT_SOURCE_DIR}/application/r_display_init.c"
  "${PROJECT_SOURCE_DIR}/application/common/camera/camera_mipi_imx219_b.c"
  "${PROJECT_SOURCE_DIR}/application/common/camera/camera_mipi_imx219_b_regdata.c"
  "${PROJECT_SOURCE_DIR}/application/common/port_settings/ceu_portsetting.c"
  "${PROJECT_SOURCE_DIR}/application/common/port_settings/vdc_portsetting.c"
  "${PROJECT_SOURCE_DIR}/hwsetup/hwsetup_userdef.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/r_lcd_controller_if.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/FT5216/lcd_ft5216.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/FT5216/lcd_ft5216_int.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/r_usb_hhid_apl.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/r_usbh0_hhid_main.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/r_usbh1_hhid_main.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/r_usb_gui_control.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/usb_gui_data/arrow_data.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/lcd_controller/usb/usb_gui_data/caution_data.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_if.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_task.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_task_riic.c"
  "${PROJECT_SOURCE_DIR}/middleware/touch/src/touch/tp_task_usb.c"
)

SET(SOURCES_LIST_RENESAS
    ${RENESAS_SRC}
	PARENT_SCOPE
)

SOURCE_GROUP("Source Files\\Renesas" FILES ${RENESAS_SRC})
