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
    The test_rtc1 interface provides a test application with functions
    operating on the RTC1 timer.
  @}

  @defgroup test_rtc1 TEST_RTC1
  @{
*/

/* Header guard */
#ifndef TEST_RTC1_H__
#define TEST_RTC1_H__

#include <stdint.h>
#include <stdbool.h>

typedef void (*irqhandler_func_t)(void);

void test_rtc1_init(void);
void test_rtc1_wait_ms(uint32_t value);

void test_rtc1_cc0_timeout_set_ms(uint32_t ms_value);
bool test_rtc1_cc0_timeout(void);
void test_rtc1_cc1_timeout_set_ms(uint32_t ms_value);
bool test_rtc1_cc1_timeout(void);
void test_rtc1_cc2_timeout_set_ms(uint32_t ms_value);
bool test_rtc1_cc2_timeout(void);

// Default timeout function (uses cc0)
void test_rtc1_timeout_set_ms(uint32_t value);
bool test_rtc1_timeout(void);

uint32_t test_rtc1_count_get(void);
uint32_t test_rtc1_diff_ms_get(uint32_t to, uint32_t from);

void test_rtc1_cc2_set_irq_handler(irqhandler_func_t func);

#endif // TEST_RTC1_H__

/**
  @}
*/
