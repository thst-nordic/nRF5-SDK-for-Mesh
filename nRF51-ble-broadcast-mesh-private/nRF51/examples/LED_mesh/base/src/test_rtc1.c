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

  @}

  The test timer libraries provided common functionality for tests running on
  the target.

  @ingroup test_rtc1 TEST_RTC1
  @{
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "test_common.h"
#include "test_rtc1.h"
#include "nrf.h"

#define M_RTC1_PRESCALER        0
#define M_RTC1_CLOCK_FREQ   32768
#define M_RTC1_COUNTER_RES  30517

#define MAX_RTC_CNT             0x00FFFFFF                                  /**< Maximum value of the RTC counter. */
#define M_MS_TO_TICKS(MS, PRESCALER)\
              ((uint32_t)ROUNDED_DIV((MS) * (uint64_t)M_RTC1_CLOCK_FREQ, ((PRESCALER) + 1) * 1000))
#define M_TICKS_TO_MS(TICKS, PRESCALER)\
              ((uint32_t)ROUNDED_DIV((TICKS) * (uint64_t)M_RTC1_COUNTER_RES, ((PRESCALER) + 1) * 1000000))


static volatile bool m_timeout_cc[3];
static volatile uint32_t m_rtc1_wrap_counter;

static volatile irqhandler_func_t mp_rtc1_irqhandler = NULL;

void test_rtc1_init(void)
{
  NVIC_DisableIRQ(RTC1_IRQn);

  NRF_RTC1->TASKS_STOP           = 1;
  NRF_RTC1->TASKS_CLEAR          = 1;
  NRF_RTC1->EVENTS_COMPARE[0]    = 0;
  NRF_RTC1->EVENTS_COMPARE[1]    = 0;
  NRF_RTC1->EVENTS_COMPARE[2]    = 0;
  NRF_RTC1->EVENTS_COMPARE[3]    = 0;
  NRF_RTC1->PRESCALER            = 0; // Wrap on 512 seconds
  NRF_RTC1->INTENCLR             = (RTC_INTENCLR_COMPARE3_Clear << RTC_INTENCLR_COMPARE3_Pos) |
                                   (RTC_INTENCLR_COMPARE2_Clear << RTC_INTENCLR_COMPARE2_Pos) |
                                   (RTC_INTENCLR_COMPARE1_Clear << RTC_INTENCLR_COMPARE1_Pos) |
                                   (RTC_INTENCLR_COMPARE0_Clear << RTC_INTENCLR_COMPARE0_Pos) |
                                   (RTC_INTENCLR_OVRFLW_Clear   << RTC_INTENCLR_OVRFLW_Pos)   |
                                   (RTC_INTENCLR_TICK_Clear     << RTC_INTENCLR_TICK_Pos) ;
  NRF_RTC1->INTENSET             = (RTC_INTENSET_OVRFLW_Set     << (RTC_INTENSET_OVRFLW_Pos))    |
                                   (RTC_INTENSET_COMPARE0_Set   << (RTC_INTENSET_COMPARE0_Pos))  |
                                   (RTC_INTENSET_COMPARE1_Set   << (RTC_INTENSET_COMPARE1_Pos))  |
                                   (RTC_INTENSET_COMPARE2_Set   << (RTC_INTENSET_COMPARE2_Pos));
  NRF_RTC1->EVTENCLR             = (RTC_EVTENCLR_COMPARE3_Clear << RTC_EVTENCLR_COMPARE3_Pos) |
                                   (RTC_EVTENCLR_COMPARE2_Clear << RTC_EVTENCLR_COMPARE2_Pos) |
                                   (RTC_EVTENCLR_COMPARE1_Clear << RTC_EVTENCLR_COMPARE1_Pos) |
                                   (RTC_EVTENCLR_COMPARE0_Clear << RTC_EVTENCLR_COMPARE0_Pos) |
                                   (RTC_EVTENCLR_OVRFLW_Clear   << RTC_EVTENCLR_OVRFLW_Pos)   |
                                   (RTC_EVTENCLR_TICK_Clear     << RTC_EVTENCLR_TICK_Pos) ;
  NRF_RTC1->EVTENSET             = (1 << (RTC_EVTEN_COMPARE0_Pos));
  NRF_RTC1->EVTENSET             = (1 << (RTC_EVTEN_COMPARE3_Pos));
  NRF_RTC1->EVTENSET             = (1 << (RTC_EVTEN_OVRFLW_Pos));

  NRF_CLOCK->TASKS_LFCLKSTOP     = 1;
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
  NRF_CLOCK->TASKS_LFCLKSTART    = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
  {
  }

  m_timeout_cc[0] = false;
  m_timeout_cc[1] = false;
  m_timeout_cc[2] = false;
  m_rtc1_wrap_counter = 0;

  NVIC_SetPriority(RTC1_IRQn, 1); // APP HIGH
  NVIC_ClearPendingIRQ(RTC1_IRQn);
  NVIC_EnableIRQ(RTC1_IRQn);                                      // Enable Interrupt for the RTC in the core.
  NRF_RTC1->TASKS_START = 1;
}


void test_rtc1_wait_ms(uint32_t ms_value)
{
  NRF_RTC1->CC[3] = (NRF_RTC1->COUNTER + M_MS_TO_TICKS(ms_value, M_RTC1_PRESCALER)) % MAX_RTC_CNT;
  NRF_RTC1->EVENTS_COMPARE[3] = 0;
  while (NRF_RTC1->EVENTS_COMPARE[3] == 0)
  {
    //wait
    // TODO: Consider doing __WFI();__WFE(); here
  }
  return;
}


void RTC1_IRQHandler(void)
{
  if ( (NRF_RTC1->EVENTS_COMPARE[0] == 1) )
  {
    NRF_RTC1->EVENTS_COMPARE[0] = 0;
    m_timeout_cc[0] = true;
  }

  if ( (NRF_RTC1->EVENTS_COMPARE[1] == 1) )
  {
    NRF_RTC1->EVENTS_COMPARE[1] = 0;
    m_timeout_cc[1] = true;
  }

  if ( (NRF_RTC1->EVENTS_COMPARE[2] == 1) )
  {
    NRF_RTC1->EVENTS_COMPARE[2] = 0;
    if (mp_rtc1_irqhandler != NULL)
    {
      (*mp_rtc1_irqhandler)();
    }
    else
    {
      m_timeout_cc[2] = true;
    }
  }

  if ( (NRF_RTC1->EVENTS_OVRFLW == 1) )
  {
    NRF_RTC1->EVENTS_OVRFLW = 0;
    m_rtc1_wrap_counter += MAX_RTC_CNT + 1;
  }
}

void test_rtc1_cc2_set_irq_handler(irqhandler_func_t func)
{
  mp_rtc1_irqhandler = func;
}

static void m_ccx_set_timeout_ms(uint32_t ms_value, uint8_t cc_index)
{
  NRF_RTC1->CC[cc_index] = (NRF_RTC1->COUNTER + M_MS_TO_TICKS(ms_value, M_RTC1_PRESCALER)) % MAX_RTC_CNT;
  NRF_RTC1->EVENTS_COMPARE[cc_index] = 0;
  m_timeout_cc[cc_index] = false;
}

void test_rtc1_cc0_timeout_set_ms(uint32_t ms_value)
{
  m_ccx_set_timeout_ms(ms_value, 0);
}

bool test_rtc1_cc0_timeout(void)
{
  return m_timeout_cc[0];
}

void test_rtc1_cc1_timeout_set_ms(uint32_t ms_value)
{
  m_ccx_set_timeout_ms(ms_value, 1);
}

bool test_rtc1_cc1_timeout(void)
{
  return m_timeout_cc[1];
}

void test_rtc1_cc2_timeout_set_ms(uint32_t ms_value)
{
  m_ccx_set_timeout_ms(ms_value, 2);
}

bool test_rtc1_cc2_timeout(void)
{
  return m_timeout_cc[2];
}

void test_rtc1_timeout_set_ms(uint32_t ms_value)
{
  test_rtc1_cc0_timeout_set_ms(ms_value);
}

bool test_rtc1_timeout(void)
{
  return test_rtc1_cc0_timeout();
}

uint32_t test_rtc1_count_get()
{
  return m_rtc1_wrap_counter + NRF_RTC1->COUNTER;
}

uint32_t test_rtc1_diff_ms_get(uint32_t to, uint32_t from)
{
  return M_TICKS_TO_MS((to - from), M_RTC1_PRESCALER);
}

/**
  @}
*/
