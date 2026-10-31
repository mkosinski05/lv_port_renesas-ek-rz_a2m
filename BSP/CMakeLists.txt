# BSP for Renesas RZ/A2M Boards
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(BSP)
SET(PROJECT_VERSION 10.0.0)

ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/src/png" ./png)
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/src/renesas" ./renesas)
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/src/freertos" ./freertos)
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/generate" ./generate)

# include folders of all sub-folder
# parent-scope does not work for includes
INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}/generate/compiler/inc"
  "${PROJECT_SOURCE_DIR}/generate/configuration"
  "${PROJECT_SOURCE_DIR}/generate/drivers/r_cache/inc"
  "${PROJECT_SOURCE_DIR}/generate/drivers/r_cpg/inc"
  "${PROJECT_SOURCE_DIR}/generate/drivers/r_gpio/inc"
  "${PROJECT_SOURCE_DIR}/generate/drivers/r_intc/inc"
  "${PROJECT_SOURCE_DIR}/generate/drivers/r_mmu/inc"
  "${PROJECT_SOURCE_DIR}/generate/drivers/r_stb/inc"
  "${PROJECT_SOURCE_DIR}/generate/os_abstraction/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_cbuffer/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_ceu/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_jcu/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_mipi/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_ostm/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/inc/inside"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/src/driver/code"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/src/driver/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/src/driver_l1/code"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/src/driver_l1/code/d0lib"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rga/src/driver_l1/code/rz"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_riic/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_rvapi/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_scifa/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh0/basic"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh0/basic/src/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh0/basic/userdef"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh0/hid"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh0/hid/src/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh0/hid/userdef"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh1/basic"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh1/basic/src/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh1/basic/userdef"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh1/hid"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh1/hid/src/inc"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_usbh1/hid/userdef"
  "${PROJECT_SOURCE_DIR}/generate/sc_drivers/r_vdc/inc"
  "${PROJECT_SOURCE_DIR}/generate/system/inc"
  "${PROJECT_SOURCE_DIR}/generate/system/iodefine"
  "${PROJECT_SOURCE_DIR}/generate/system/iodefine/iobitmasks"
  "${PROJECT_SOURCE_DIR}/generate/system/iodefine/iodefines"
  "${PROJECT_SOURCE_DIR}/src/config_files"
  "${PROJECT_SOURCE_DIR}/src/freertos/include"
  "${PROJECT_SOURCE_DIR}/src/freertos/include/private"
  "${PROJECT_SOURCE_DIR}/src/freertos/portable/gcc/arm_ca9_rza2m"
  "${PROJECT_SOURCE_DIR}/src/png/libpng"
  "${PROJECT_SOURCE_DIR}/src/png/zlib"
  "${PROJECT_SOURCE_DIR}/src/renesas/application/common/camera"
  "${PROJECT_SOURCE_DIR}/src/renesas/application/inc"
  "${PROJECT_SOURCE_DIR}/src/renesas/application/inc/camera"
  "${PROJECT_SOURCE_DIR}/src/renesas/application/inc/lcd"
  "${PROJECT_SOURCE_DIR}/src/renesas/middleware/touch/inc"
  "${PROJECT_SOURCE_DIR}/src/renesas/middleware/touch/src/lcd_controller/FT5216"
  "${PROJECT_SOURCE_DIR}/src/renesas/middleware/touch/src/lcd_controller/usb"
  "${PROJECT_SOURCE_DIR}/src/renesas/middleware/touch/src/lcd_controller/usb/inc"
  "${PROJECT_SOURCE_DIR}/src/renesas/middleware/touch/src/lcd_controller/usb/mouse_data"
  "${PROJECT_SOURCE_DIR}/src/renesas/middleware/touch/src/touch"
)

# ========== Build Guiliani library ==========
ADD_LIBRARY(BSP STATIC ${SOURCES_LIST_FREERTOS} ${SOURCES_LIST_PNG} ${SOURCES_LIST_RENESAS} ${SOURCES_LIST_GENERATE})
