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
    The test_timer2 interface provides a test application with functions
    operating on the TIMER2 multi timer.
  @}

  @ingroup test_timer2 TEST_TIMER2
  @{
*/

/* Header guard */
#ifndef TEST_TIMER2_H__
#define TEST_TIMER2_H__

#include <stdint.h>
#include <stdbool.h>
#include "test_common.h"

#define TEST_TIMER2_PRESCALER_1US  4 /* equals a tickrate of  1us, wraps around after 0xFFFF * 1us == 65.535ms */
#define TEST_TIMER2_PRESCALER_32US 9 /* equals a tickrate of 32us, wraps around after 0xFFFF * 32us == 3.276750s */

#define TEST_TIMER2_TICK_TO_MS(TICKS, PRESCALER) \
                ((uint32_t)ROUNDED_DIV((TICKS) * ((uint64_t)1<<(PRESCALER)), 16))

void test_timer2_init(uint32_t prescaler, bool irq_enable);
void test_timer2_start(uint32_t time);
void test_timer2_stop(void);

void test_timer2_set_count(uint32_t timer_count);
uint32_t test_timer2_get_count(void);
bool test_timer2_get_timeout(void);

uint32_t test_timer2_capture(void);

#endif /* TEST_TIMER2_H__ */

/**
  @}
*/
