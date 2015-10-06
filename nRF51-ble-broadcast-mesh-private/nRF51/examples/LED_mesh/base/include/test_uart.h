#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdbool.h>

#include "test_console.h"

#define UART_NEWLINE_INPUT      CONSOLE_NEWLINE_INPUT
#define UART_NEWLINE_OUTPUT     CONSOLE_NEWLINE_OUTPUT

/** @brief The baudrate devisors array, calculated for standard baudrates.
    Number of elements defined by ::HAL_UART_BAUD_TABLE_MAX_SIZE*/
#define HAL_UART_BAUDRATE_DEVISORS_ARRAY    { \
    0x0009D000, 0x0013B000, 0x00275000, 0x003B0000, 0x004EA000, 0x0075F000, 0x009D5000, \
    0x00EBF000, 0x013A9000, 0x01D7E000, 0x03AFB000, 0x04000000, 0x075F7000, 0x10000000  }

/** Available Baud rates for UART. */
typedef enum
{
    HAL_UART_BAUD_2K4 = 0,        ///< 2400 baud
    HAL_UART_BAUD_4K8,            ///< 4800 baud
    HAL_UART_BAUD_9K6,            ///< 9600 baud
    HAL_UART_BAUD_14K4,           ///< 14.4 kbaud
    HAL_UART_BAUD_19K2,           ///< 19.2 kbaud
    HAL_UART_BAUD_28K8,           ///< 28.8 kbaud
    HAL_UART_BAUD_38K4,           ///< 38.4 kbaud
    HAL_UART_BAUD_57K6,           ///< 57.6 kbaud
    HAL_UART_BAUD_76K8,           ///< 76.8 kbaud
    HAL_UART_BAUD_115K2,          ///< 115.2 kbaud
    HAL_UART_BAUD_230K4,          ///< 230.4 kbaud
    HAL_UART_BAUD_250K0,          ///< 250.0 kbaud
    HAL_UART_BAUD_460K8,          ///< 460.8 kbaud
    HAL_UART_BAUD_1M0,            ///< 1 mbaud
    HAL_UART_BAUD_TABLE_MAX_SIZE  ///< Used to specify the size of the baudrate table.
} uart_baudrate_t;

/** @brief This function initializes and starts the UART.
 *
 * @param baud_rate - the baud rate to be used, ::uart_baudrate_t.
 *
 */
void test_uart_init(uart_baudrate_t const baud_rate);

/** @brief This function starts the UART
 *
 */
void test_uart_enable(void);

/** @brief This function stops the UART
 *
 */
void test_uart_disable(void);

/** @brief Find number of characters in the UART receive buffer
 *
 * This function returns the number of characters available for reading
 * in the UART receive buffer.
 *
 * @return Number of characters available
 */
bool test_uart_chars_available(void);

/** Function to write a character to the UART transmit buffer.
 * @param ch Character to write
 */
void test_uart_putchar(uint8_t ch);

/** Function to read a character from the UART receive buffer.
 * @return Character read
 */
uint8_t test_uart_getchar(void);

#endif // __UART_H__
