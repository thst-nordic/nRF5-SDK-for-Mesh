
/*
 * Copyright (c) Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
/**
  @defgroup test_console TEST_CONSOLE
  @{

  @file

  @brief Implementation of test_test_console.h.
*/


#include "test_console.h"
#include "test_uart.h"

#define RETURN_IF_CONSOLE_IS_NA  { if (m_console != CONSOLE_AVAILABLE) return; }

#ifdef CONSOLE_ENABLE_ECHO
  #define ECHO(c) { test_uart_putchar(*(chars-1)); }
#else
  #define ECHO(c) {}
#endif

static const char hex_tab[] = "0123456789ABCDEF";    /* Table of ASCII hexadecimal digits */

static enum
{
  CONSOLE_UNINIT = 0,
  CONSOLE_AVAILABLE
} m_console = CONSOLE_UNINIT;

void test_console_init(void)
{
  test_uart_init(HAL_UART_BAUD_19K2);
  m_console = CONSOLE_AVAILABLE;
}

bool test_console_available(void)
{
  if ( m_console == CONSOLE_AVAILABLE )
  {
    return true;
  }
  else
  {
    return false;
  }
}

void test_console_put_string(uint8_t const * str)
{
  RETURN_IF_CONSOLE_IS_NA;
  while (*str != 0)
  {
    test_uart_putchar(*str++);
  }
}

void test_console_put_line(uint8_t const * str)
{
  test_console_put_string(str);
  test_console_put_newline();
}

void test_console_put_newline(void)
{
  RETURN_IF_CONSOLE_IS_NA;
  test_console_put_char(CONSOLE_NEWLINE_OUTPUT);
}

void test_console_put_chars(uint8_t const * chars, uint8_t num_chars)
{
  RETURN_IF_CONSOLE_IS_NA;
  for( ; num_chars > 0; num_chars--)
  {
    test_uart_putchar(*chars++);
  }
}

void test_console_get_string(uint8_t * str, uint8_t num_chars)
{
  RETURN_IF_CONSOLE_IS_NA;
  for( ; num_chars > 0; num_chars--)
  {
    *str++ = test_uart_getchar();
    ECHO(*(str-1));
  }
  *str = 0;   /* Add zero terminator */
}

void test_console_get_line(uint8_t * str, uint8_t max_len)
{
  uint8_t c, k;

  RETURN_IF_CONSOLE_IS_NA;
  c = '\0';
  for( k = 0; k < max_len - 1 ; k++ )
  {
    c = test_uart_getchar();
    if (CONSOLE_NEWLINE_INPUT == c)
    {
      break;
    }
    str[k] = c;
    ECHO(c);
  }
  str[k] = 0;
} //lint !e438 Last value assigned to 'c' not used

bool test_console_chars_available(void)
{
  return test_uart_chars_available();
}

void test_console_get_chars(uint8_t * chars, uint8_t num_chars)
{
  RETURN_IF_CONSOLE_IS_NA;
  for( ; num_chars > 0; num_chars--)
  {
    *chars++ = test_uart_getchar();
    ECHO(*(chars-1));
  }
}

void test_console_put_char(uint8_t ch)
{
  RETURN_IF_CONSOLE_IS_NA;
  test_uart_putchar(ch);
}

uint8_t test_console_get_char(void)
{
  uint8_t ch = '\0';

  if ( m_console == CONSOLE_AVAILABLE )
  {
    ch = test_uart_getchar();
    ECHO(ch);
  }
  return ch;
}

static void test_console_put_uint(uint32_t val)
{
  uint8_t i = 0;
  uint8_t char_vals[10] = {0};

  RETURN_IF_CONSOLE_IS_NA;
  do
  {
    char_vals[i++] = val % 10;
    val /= 10;
  } while (val != 0);

  for (; i > 0; i--)
  {
    test_uart_putchar(char_vals[i - 1] + '0');
  }
}

void test_console_put_decbyte(uint8_t b) // b is in the range [0 255]
{
  test_console_put_uint(b);
}

void test_console_put_decword(uint16_t w)  // w is in the range [0 65535]
{
  test_console_put_uint(w);
}

void test_console_put_dec32bit(uint32_t ww)  // ww is in the range [0 4294967295]
{
  test_console_put_uint(ww);
}

void test_console_put_hexnybble(uint8_t n)
{
  RETURN_IF_CONSOLE_IS_NA;
  test_uart_putchar((uint8_t)hex_tab[n & 0x0f]);
}

void test_console_put_hexbyte(uint8_t b)
{
  test_console_put_hexnybble(b >> 4);
  test_console_put_hexnybble(b & 0x0f);
}

void test_console_put_hexword(uint32_t w)
{
  test_console_put_hexbyte((uint8_t)(w >> 24));
  test_console_put_hexbyte((uint8_t)(w >> 16));
  test_console_put_hexbyte((uint8_t)(w >> 8));
  test_console_put_hexbyte(w & 0xff);
}

void test_console_put_hexbytearray(uint8_t* p, uint8_t n)
{
  uint8_t i;
  if (n > 0)
  {
    test_console_put_hexbyte(p[0]);
  }
  for (i = 1; i < n; ++i)
  {
    test_console_put_string((uint8_t *)"-");
    test_console_put_hexbyte(p[i]);
  }
}

uint8_t test_console_get_hexnybble(void)
{
  uint8_t c;
  if ( m_console == CONSOLE_AVAILABLE )
  {
    c = test_uart_getchar();
    ECHO(c);
    if (c >= '0' && c <= '9')
    {
      return c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
      return (c - 'a') + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
      return (c - 'A') + 10;
    }
    else
    {
      /** @note We have encountered something that is not a hexadecimal digit.
       * This is an error, and it is not reported.  An ASSERT(false) would be suitable here. */
      return 0;
    }
  }

  return 0;   /// @note This may not be the correct value, what should be returned if m_console != CONSOLE_AVAILABLE ?
}

uint8_t test_console_get_hexbyte(void)
{
  uint8_t nh = test_console_get_hexnybble();
  return (uint8_t)((nh << 4) | test_console_get_hexnybble());
}

uint16_t test_console_get_hexword(void)
{
  uint16_t bh = test_console_get_hexbyte();
  return (uint16_t)((bh << 8) | test_console_get_hexbyte());
}

void test_console_flush(void)
{
  while (test_console_chars_available())
  {
    (void) test_console_get_char(); // flush input buffer
  }
}

void test_console_temporary_disable(void)
{
  test_uart_disable();
}

void test_console_temporary_reenable(void)
{
  test_uart_enable();
}


/**
  @}
*/
