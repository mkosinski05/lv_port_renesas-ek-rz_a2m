# FreeRTOS for Renesas RZ/A2M Boards
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(RZA2M_FREERTOS)
SET(PROJECT_VERSION 10.0.0)

SET(FREERTOS_INC
  "${PROJECT_SOURCE_DIR}/include/croutine.h"
  "${PROJECT_SOURCE_DIR}/include/event_groups.h"
  "${PROJECT_SOURCE_DIR}/include/FreeRTOS.h"
  "${PROJECT_SOURCE_DIR}/include/message_buffer.h"
  "${PROJECT_SOURCE_DIR}/include/queue.h"
  "${PROJECT_SOURCE_DIR}/include/semphr.h"
  "${PROJECT_SOURCE_DIR}/include/stream_buffer.h"
  "${PROJECT_SOURCE_DIR}/include/task.h"
  "${PROJECT_SOURCE_DIR}/include/timers.h"
  "${PROJECT_SOURCE_DIR}/include/private/deprecated_definitions.h"
  "${PROJECT_SOURCE_DIR}/include/private/list.h"
  "${PROJECT_SOURCE_DIR}/include/private/mpu_wrappers.h"
  "${PROJECT_SOURCE_DIR}/include/private/portable.h"
  "${PROJECT_SOURCE_DIR}/include/private/projdefs.h"
  "${PROJECT_SOURCE_DIR}/include/private/stack_macros.h"
  "${PROJECT_SOURCE_DIR}/include/private/threading_alt.h"
  "${PROJECT_SOURCE_DIR}/portable/gcc/arm_ca9_rza2m/portmacro.h"
)

SET(HEADERS_LIST_FREERTOS
    ${FREERTOS_INC}
    PARENT_SCOPE
)

SOURCE_GROUP("Header Files\\FreeRTOS10" FILES ${FREERTOS_INC})

SET(FREERTOS_SRC
  "${PROJECT_SOURCE_DIR}/event_groups.c"
  "${PROJECT_SOURCE_DIR}/list.c"
  "${PROJECT_SOURCE_DIR}/queue.c"
  "${PROJECT_SOURCE_DIR}/stream_buffer.c"
  "${PROJECT_SOURCE_DIR}/tasks.c"
  "${PROJECT_SOURCE_DIR}/timers.c"
  "${PROJECT_SOURCE_DIR}/portable/gcc/arm_ca9_rza2m/freertos_tick_config.c"
  "${PROJECT_SOURCE_DIR}/portable/gcc/arm_ca9_rza2m/port.c"
  "${PROJECT_SOURCE_DIR}/portable/gcc/arm_ca9_rza2m/3rt_access.S"
  "${PROJECT_SOURCE_DIR}/portable/gcc/arm_ca9_rza2m/portasm.S"
)

SET(SOURCES_LIST_FREERTOS
    ${FREERTOS_SRC}
    PARENT_SCOPE
)

SOURCE_GROUP("Source Files\\FreeRTOS10" FILES ${FREERTOS_SRC})
