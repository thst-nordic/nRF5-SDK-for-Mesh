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
    Header file for common interface functions used in all test frameworks.
  @}

  @defgroup test_util TEST_UTIL
  @{
*/

#ifndef TEST_UTIL_H__
#define TEST_UTIL_H__

#include <stdint.h>
#include <stdbool.h>

#include "test_rtc1.h"

#define MSG_ACK                      (0x0D)
#define TEST_BENCH_DEVICE_READY      (0x52)
#define TEST_BENCH_START_BYTE        (0x5A)
#define TEST_BENCH_SYNC_WAIT_START   (0x53) /* S */
#define TEST_BENCH_SYNC_WAIT_STOP    (0x43) /* C */
#define TEST_BENCH_SYNC_DATA_RECEIVE (0x41) /* A */
#define TEST_BENCH_SYNC_DATA_SEND    (0x42) /* B */ 
#define TEST_BENCH_SYNC_DATA_WAIT    (0x57) /* W */

#define TEST_BENCH_MSG_NONE          (0x00)

/**
  @brief
    Initialise test utility - must be called prior to any other interface function.
*/
void test_util_init(void);

/**
  @brief
    Blocking function to wait for testbench to indicate test start over uart.
*/
void test_util_wait_test_bench_start(void);

void test_util_timeout_set_32us(uint32_t value);
void test_util_wait_32us(uint32_t value);

bool test_util_timeout(void);
uint32_t test_util_timeout_get_s(void);
uint32_t test_util_timeout_get_ms(void);

uint8_t test_util_message_next_(const char * msg_txt);
void test_util_message_wait_(uint8_t wait_msg, const char * msg_txt);
void test_util_message_send_(uint8_t message, const char * msg_txt); 

#define TEST_UTIL_MSG_ACK()\
  test_util_message_send_(MSG_ACK, "MSG_ACK"); 
  
#define TEST_UTIL_MSG_NEXT(msg)\
  test_util_message_next_(#msg);\
  test_util_message_send_(MSG_ACK, "MSG_ACK"); 

/** @note Try to use minimum synchronisation between tester and DUT over UART, 
    as the UART driver is a polling driver and the Rx has just one byte buffer
    (no software buffer used for Rx) */
#define TEST_UTIL_MSG_SEND(msg)\
  test_rtc1_timeout_set_ms(3000);\
  test_util_message_send_(msg, #msg);\
  test_util_message_wait_(MSG_ACK, "MSG_ACK");

#define TEST_UTIL_MSG_WAIT(msg)\
  test_util_message_wait_(msg, #msg);\
  test_util_message_send_(MSG_ACK, "MSG_ACK"); 

#define TEST_UTIL_ASYNC_MSG_SEND(msg)\
  test_util_message_send_(msg, #msg);

/** @brief Receive message and match with the expected message */
bool test_util_receive_and_match(uint8_t message);

#define TEST_UTIL_MSG_RECV(msg)\
  test_util_receive_and_match(msg)

void    test_util_send_message(uint8_t message);
uint8_t test_util_receive_message(void);

/** Open tag for generic info.
 * This tag, together with its associated end tag can be used anywhere
 * in the report to add generic comments or information.
 */
void test_util_tag_open(const char * tag_string);

/* Close the last xml tag*/
void test_util_tag_close(void);

void test_util_tag_open_param_str(const char * tag_string, const char * param_name, const char * param_string);
void test_util_tag_open_param_u16(const char * tag_string, const char * param_name, uint16_t u16);
void test_util_tag_open_param_u8(const char * tag_string, const char * param_name, unsigned char u8);

/* To be implemented by the caller of the test util, this job callback allows
  the caller to do job tasks while the test util is in a wait state
*/
void test_util_job_callback(void);

void test_util_log_info(const uint8_t* msg);

void test_util_sync_with_message(uint8_t const * message);
void test_util_sync_with_string_data(uint8_t const * metadata, uint8_t* data);
void test_util_sync_with_message_and_data(uint8_t const * metadata, uint8_t* data);
void test_util_sync_uint16(uint16_t handle, uint8_t *p_tag);
void test_util_sync_and_get_data(uint8_t * buffer, uint8_t * result_length);

void test_util_put_uint32(uint32_t input, uint8_t const * xml_tag);
void test_util_put_uint32_hex(uint32_t input, uint8_t const * xml_tag);

#endif /* TEST_UTIL_H__ */

/**
  @}
*/
