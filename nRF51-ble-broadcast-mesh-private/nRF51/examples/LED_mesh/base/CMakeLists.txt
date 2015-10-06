set(NRF51_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/stack/deploy/internal/include/nrf51")
set(NRF52_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/stack/deploy/internal/include/nrf52")

include_directories(
  "${PROJECT_SOURCE_DIR}/stack/deploy/internal/include"
)

set(LIB_SRCS
    src/test_flash_assert.c
    src/test_console.c
    src/test_evt.c
    src/test_rtc1.c
    src/test_sds.c
    src/test_timer2.c
    src/test_uart.c
    src/test_util.c
)

add_library_platform(base ${LIB_SRCS})
