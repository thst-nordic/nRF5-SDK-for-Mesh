/*
 * Copyright (c) Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
/** @file

  @brief @{
    Implementation of the highest level routines in the test common interface.
  @}

  The test util library provides basic functionality for tests running on
  the target.

  @ingroup test_util TEST_UTIL
  @{
*/

#include <stdint.h>
#include <stdbool.h>

#include "test_util.h"
#include "test_util.h"
#include "test_timer2.h"
#include "test_console.h"

#include "nrf.h"

#define TEST_BENCH_EVENT_TIMEOUT     (5) /* seconds */
#define M_PRESCALER                  TEST_TIMER2_PRESCALER_32US

static const char * xml_tag_string;

void test_util_wait_32us(uint32_t value)
{
  test_util_timeout_set_32us(value);
  while ( !test_timer2_get_timeout() )
  {
  }
}

void test_util_timeout_set_32us(uint32_t value)
{
  if (value == 0)
  {
    test_timer2_set_count(0);
    return;
  }
  
  test_timer2_init(M_PRESCALER, true);
  test_timer2_set_count(1);
  test_timer2_start(value);
}

bool test_util_timeout(void)
{
  return test_timer2_get_timeout();
}

/** @brief Block (wait) until testbench is started with received UART byte. 
*/
void test_util_wait_test_bench_start(void)
{
  bool done = false;
  uint8_t start_byte;

  /* Flush the characters received at UART, if any */
  while(test_console_chars_available())
  {
    start_byte = test_console_get_char();
  }

  test_console_put_char((uint8_t)TEST_BENCH_DEVICE_READY);

  do
  {
    while ( !test_console_chars_available() )
      {
      }

    test_console_get_chars(&start_byte, 1);
    test_console_put_char(start_byte);
    if ( start_byte == TEST_BENCH_START_BYTE )
      {
      done = true;
      }
  } while ( !done );
}

/** @breif initialise the test util.
  - Initilialise the UART port required for the test interface.
*/
void test_util_init(void)
{
  test_console_init();
}

void test_util_send_message(uint8_t message)
{
  test_util_tag_open("message");
  test_console_put_string((const uint8_t *)"0x");
  test_console_put_hexbyte(message);
  test_util_tag_close();
}

void test_util_message_send_(uint8_t message, const char * msg_txt)
{
  test_util_tag_open("msg_tx");
  test_console_put_string((uint8_t *)msg_txt);
  test_console_put_string((const uint8_t *)"(0x");
  test_console_put_hexbyte(message);
  test_console_put_string((const uint8_t *)")");
  test_util_tag_close();

  test_util_tag_open("message");
  test_console_put_string((const uint8_t *)"0x");
  test_console_put_hexbyte(message);
  test_util_tag_close();
}

static bool test_util_receive_next(uint8_t * message)
{
  if( test_console_chars_available() )
  {
    test_console_get_chars(message, 1);
    test_util_tag_open("msg_rx");
    test_console_put_string((const uint8_t *)"0x");
    test_console_put_hexbyte(*message);
    test_util_tag_close();
    return true;
  }
  else
  {
    *message = TEST_BENCH_MSG_NONE;
    return false;
  }
}

uint8_t test_util_receive_message(void)
{
  uint8_t rx_msg;

  if (!test_util_receive_next(&rx_msg))
  {
    rx_msg = TEST_BENCH_MSG_NONE;
  }
  return rx_msg;
}

bool test_util_receive_and_match(uint8_t message)
{
  uint8_t rx_msg;

  if ( test_util_receive_next(&rx_msg) )
  {
    if ( rx_msg == message )
    {
      return true;
    }
  }
  else
  {
    return false;
  }
  return false; //lint !e527 Unreachable code
}

uint8_t test_util_message_next_(const char * msg_text)
{
  uint8_t rx_msg = TEST_BENCH_MSG_NONE;

  test_util_tag_open("msg_wait");
  test_console_put_string((const uint8_t *)msg_text);
  test_console_put_string((const uint8_t *)"(0xXX)");
  test_util_tag_close();

  while ( !test_util_receive_next(&rx_msg) )
  {
  }

  return rx_msg;
}

void test_util_message_wait_(uint8_t wait_msg, const char * msg_txt)
{
  uint8_t rx_msg = TEST_BENCH_MSG_NONE;

  test_util_tag_open("msg_wait");
  test_console_put_string((const uint8_t *)msg_txt);
  test_console_put_string((const uint8_t *)"(0x");
  test_console_put_hexbyte(wait_msg);
  test_console_put_string((const uint8_t *)")");
  test_util_tag_close();

  do
  {
    (void)test_util_receive_next(&rx_msg);
  } while ( rx_msg != wait_msg );
}

void test_util_tag_open(const char * tag_string)
{
  xml_tag_string = tag_string;
  test_console_put_string((const uint8_t *)"      <");
  test_console_put_string((const uint8_t *)xml_tag_string);
  test_console_put_char('>');
}

void test_util_tag_open_param_str(const char * tag_string, const char * param_name, const char * param_string)
{
  xml_tag_string = tag_string;
  test_console_put_string((const uint8_t *)"      <");
  test_console_put_string((const uint8_t *)xml_tag_string);
  test_console_put_char(' ');
  test_console_put_string((const uint8_t *)param_name);
  test_console_put_char('=');
  test_console_put_char('"');
  test_console_put_string((const uint8_t *)param_string);
  test_console_put_char('"');
  test_console_put_char('>');
}

void test_util_tag_open_param_u8(const char * tag_string, const char * param_name, unsigned char u8)
{
  xml_tag_string = tag_string;
  test_console_put_string((const uint8_t *)"      <");
  test_console_put_string((const uint8_t *)xml_tag_string);
  test_console_put_char(' ');
  test_console_put_string((const uint8_t *)param_name);
  test_console_put_char('=');
  test_console_put_char('"');
  test_console_put_decbyte(u8);
  test_console_put_char('"');
  test_console_put_char('>');
}

void test_util_tag_open_param_u16(const char * tag_string, const char * param_name, uint16_t u16)
{
  xml_tag_string = tag_string;
  test_console_put_string((const uint8_t *)"      <");
  test_console_put_string((const uint8_t *)xml_tag_string);
  test_console_put_char(' ');
  test_console_put_string((const uint8_t *)param_name);
  test_console_put_char('=');
  test_console_put_char('"');
  test_console_put_decword(u16);
  test_console_put_char('"');
  test_console_put_char('>');
}

void test_util_tag_close(void)
{
  test_console_put_char('<');
  test_console_put_char('/');
  test_console_put_string((const uint8_t *)xml_tag_string);
  test_console_put_char('>');
  test_console_put_newline();
}

static void _test_util_wait_for_continue(void)
{
  uint8_t sync_byte;
  bool done = false;

  do
  {
    while ( !test_console_chars_available() )
    {
    }

    test_console_get_chars(&sync_byte, 1);
    if ( sync_byte == TEST_BENCH_SYNC_WAIT_STOP )
    {
      done = true;
    }
  } while ( !done );
}

void test_util_sync_with_message(uint8_t const * message)
{
  test_console_put_char(TEST_BENCH_SYNC_WAIT_START);
  test_console_put_newline();
  test_console_put_string(message);
  test_console_put_newline();

  _test_util_wait_for_continue();
} 

void test_util_sync_with_string_data(uint8_t const * metadata, uint8_t* data)
{
  test_console_put_char(TEST_BENCH_SYNC_DATA_RECEIVE);
  //test_console_put_char(TEST_BENCH_SYNC_WAIT_START);
  test_console_put_newline();
  test_util_tag_open((const char*) metadata);
  test_console_put_string(data);
  test_util_tag_close();

  _test_util_wait_for_continue();
}

void test_util_sync_with_message_and_data(uint8_t const * message, uint8_t* data)
{
  test_console_put_char(TEST_BENCH_SYNC_WAIT_START);
  test_console_put_newline();
  test_console_put_string(message);
  test_console_put_string(" ");
  test_console_put_string(data);
  test_console_put_newline();

  _test_util_wait_for_continue();
}

static void s_populate_string_handle(uint16_t handle, uint8_t *p_data)
{
  uint8_t pos = 3;
  uint8_t char_array[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  do{
    p_data[3-pos] = char_array[(handle >> (4*pos)) & 0x0F];
  }while(pos--);
}

void test_util_sync_uint16(uint16_t handle, uint8_t *p_tag)
{
  uint8_t string_handle[5] = {0};

  s_populate_string_handle(handle, string_handle);
  test_util_sync_with_string_data(p_tag, string_handle);
}

void test_util_sync_and_get_data(uint8_t * buffer, uint8_t * result_length)
{ 
  test_console_put_char(TEST_BENCH_SYNC_DATA_WAIT); /* Send request for data */
  test_console_put_newline();
  *result_length = test_console_get_hexbyte(); /* Get 2 bytes of length followed by the data */
  test_console_get_chars(buffer, *result_length);
  
  _test_util_wait_for_continue();
}

void test_util_put_uint32(uint32_t input, uint8_t const * xml_tag)
{
  int j;
  uint32_t scopy;
  uint8_t buf[11] = "xxxxxxxxxx";
  
  scopy = input;
  for (j = 9; j >= 0; j--) {
    uint8_t digit = scopy % 10;
    scopy = scopy / 10;
    buf[j] = '0' + digit;
  }
  
  test_util_tag_open((const char*)xml_tag);
  test_console_put_string(buf);
  test_util_tag_close();
}

void test_util_put_uint32_hex(uint32_t input, uint8_t const * xml_tag)
{
  test_util_tag_open((const char*)xml_tag);
  test_console_put_char('0');
  test_console_put_char('x');
  test_console_put_hexbyte((uint8_t)(input >> 24) & 0x00000FF);
  test_console_put_hexbyte((uint8_t)(input >> 16) & 0x00000FF);
  test_console_put_hexbyte((uint8_t)(input >>  8) & 0x00000FF);
  test_console_put_hexbyte((uint8_t)(input      ) & 0x00000FF);
  test_util_tag_close();
}

void test_util_log_info(const uint8_t* msg)
{
  test_util_tag_open("info");
  test_console_put_string(msg);
  test_util_tag_close();
}


/**
  @}
*/
