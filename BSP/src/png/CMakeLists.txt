# FreeRTOS for Renesas RZ/A2M Boards
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(RZA2M_PNG)
SET(PROJECT_VERSION 10.0.0)

SET(PNG_INC
  "${PROJECT_SOURCE_DIR}/zlib/zutil.h"
  "${PROJECT_SOURCE_DIR}/zlib/zlib.h"
  "${PROJECT_SOURCE_DIR}/zlib/zconf.h"
  "${PROJECT_SOURCE_DIR}/zlib/inftrees.h"
  "${PROJECT_SOURCE_DIR}/zlib/inflate.h"
  "${PROJECT_SOURCE_DIR}/zlib/inffixed.h"
  "${PROJECT_SOURCE_DIR}/zlib/inffast.h"
  "${PROJECT_SOURCE_DIR}/zlib/crc32.h"
  "${PROJECT_SOURCE_DIR}/libpng/pngstruct.h"
  "${PROJECT_SOURCE_DIR}/libpng/pngpriv.h"
  "${PROJECT_SOURCE_DIR}/libpng/pnglibconf.h"
  "${PROJECT_SOURCE_DIR}/libpng/pnginfo.h"
  "${PROJECT_SOURCE_DIR}/libpng/pngdebug.h"
  "${PROJECT_SOURCE_DIR}/libpng/pngconf.h"
  "${PROJECT_SOURCE_DIR}/libpng/png.h"
)

SET(HEADERS_LIST_PNG
    ${PNG_INC}
    PARENT_SCOPE
)

SOURCE_GROUP("Header Files\\PNG" FILES ${PNG_INC})

SET(PNG_SRC
  "${PROJECT_SOURCE_DIR}/libpng/png.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngerror.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngget.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngmem.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngread.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngrio.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngrtran.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngrutil.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngset.c"
  "${PROJECT_SOURCE_DIR}/libpng/pngtrans.c"
  "${PROJECT_SOURCE_DIR}/libpng/arm/arm_init.c"
  "${PROJECT_SOURCE_DIR}/libpng/arm/filter_neon_intrinsics.c"
  "${PROJECT_SOURCE_DIR}/libpng/arm/filter_neon.S"
  "${PROJECT_SOURCE_DIR}/zlib/adler32.c"
  "${PROJECT_SOURCE_DIR}/zlib/crc32.c"
  "${PROJECT_SOURCE_DIR}/zlib/inffast.c"
  "${PROJECT_SOURCE_DIR}/zlib/inflate.c"
  "${PROJECT_SOURCE_DIR}/zlib/inftrees.c"
  "${PROJECT_SOURCE_DIR}/zlib/zutil.c"
)

SET(SOURCES_LIST_PNG
    ${PNG_SRC}
    PARENT_SCOPE
)

SOURCE_GROUP("Source Files\\PNG" FILES ${PNG_SRC})
