//#include "nrf_cunit.h"
#include "test_uart.h"
#include "nrf.h"

#ifdef NRF51
#define BOARD_UART0_RTS 8
#define BOARD_UART0_TX  9
#define BOARD_UART0_CTS 10
#define BOARD_UART0_RX  11
#endif
#ifdef NRF52
#define BOARD_UART0_RTS 5
#define BOARD_UART0_TX  6
#define BOARD_UART0_CTS 7
#define BOARD_UART0_RX  8
#endif

static const uint32_t m_baudrates[HAL_UART_BAUD_TABLE_MAX_SIZE] = HAL_UART_BAUDRATE_DEVISORS_ARRAY;

void sendchar(uint8_t ch) /* Expected found in Serial.c, used by nrf_cunit. See http://www.keil.com/support/man/docs/gsac/gsac_lowlevelroutinescortex.htm */
{
  test_uart_putchar(ch);
}

int getkey(void)          /* Expected found in Serial.c, used by nrf_cunit. See http://www.keil.com/support/man/docs/gsac/gsac_lowlevelroutinescortex.htm */
{
  return (int)test_uart_getchar();
}

void test_uart_init(uart_baudrate_t const baud_rate)
{
  volatile uint32_t write_buf_flush_dummy; //lint -e550 Symbol not accessed
  NRF_UART0->ENABLE           = 0x04;

  NRF_UART0->PSELRTS = BOARD_UART0_RTS;
  NRF_UART0->PSELTXD = BOARD_UART0_TX;
  NRF_UART0->PSELCTS = BOARD_UART0_CTS;
  NRF_UART0->PSELRXD = BOARD_UART0_RX;

  NRF_UART0->BAUDRATE         = m_baudrates[baud_rate];
  NRF_UART0->CONFIG           = (UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
  NRF_UART0->TASKS_STARTTX    = 1;
  NRF_UART0->TASKS_STARTRX    = 1;
  NRF_UART0->EVENTS_RXDRDY    = 0;
  write_buf_flush_dummy       = NRF_UART0->EVENTS_RXDRDY;
 
}

void test_uart_enable()
{
  NRF_UART0->ENABLE        = 0x04;
  NRF_UART0->TASKS_STARTTX = 1;
  NRF_UART0->TASKS_STARTRX = 1;
}

void test_uart_disable()
{
  NRF_UART0->ENABLE       = 0x00;
  NRF_UART0->TASKS_STOPRX = 1;
  NRF_UART0->TASKS_STOPTX = 1;
}

bool test_uart_chars_available(void)
{
  return (NRF_UART0->EVENTS_RXDRDY == 1);
}

void test_uart_putchar(uint8_t ch)
{
  volatile uint32_t write_buf_flush_dummy; //lint -e550 Symbol not accessed
  NRF_UART0->EVENTS_TXDRDY = 0;
  write_buf_flush_dummy = NRF_UART0->EVENTS_TXDRDY;
  NRF_UART0->TXD = ch;
  while(NRF_UART0->EVENTS_TXDRDY != 1){}  // Wait for TXD data to be sent
  NRF_UART0->EVENTS_TXDRDY = 0;
  write_buf_flush_dummy = NRF_UART0->EVENTS_TXDRDY;
}

uint8_t test_uart_getchar(void)
{
  volatile uint32_t write_buf_flush_dummy; //lint -e550 Symbol not accessed
  while(NRF_UART0->EVENTS_RXDRDY != 1){}   // Wait for RXD data to be received
  NRF_UART0->EVENTS_RXDRDY = 0;
  write_buf_flush_dummy = NRF_UART0->EVENTS_RXDRDY;
  return (uint8_t)NRF_UART0->RXD;
}

