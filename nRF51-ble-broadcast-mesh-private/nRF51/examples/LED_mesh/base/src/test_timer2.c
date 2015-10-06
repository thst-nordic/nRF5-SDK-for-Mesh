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
    Implementation of the functions operating on the TIMER2 multi timer.
  @}

  The test util libraries provided common functionality for tests running on
  the target.

  @ingroup test_timer2 TEST_TIMER2
  @{
*/

#include "test_timer2.h"
#include "nrf.h"

static volatile bool m_timeout;
static uint32_t m_timer_count;

void test_timer2_init(uint32_t prescaler, bool irq_enable)
{
  NRF_TIMER2->TASKS_STOP        = 1;
  NRF_TIMER2->TASKS_CLEAR       = 1;
  NRF_TIMER2->INTENCLR          = 0xFFFFFFFFUL;
  NRF_TIMER2->MODE              = TIMER_MODE_MODE_Timer;
  NRF_TIMER2->EVENTS_COMPARE[0] = 0;
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;
  NRF_TIMER2->EVENTS_COMPARE[2] = 0;
  NRF_TIMER2->EVENTS_COMPARE[3] = 0;
  NRF_TIMER2->SHORTS            = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos; // TODO: Hmmmm?
  NRF_TIMER2->PRESCALER         = prescaler;
  NRF_TIMER2->BITMODE           = TIMER_BITMODE_BITMODE_16Bit;
  NRF_TIMER2->INTENCLR          = 0xFFFFFFFF;

  if (irq_enable)
  {
    NRF_TIMER2->INTENSET          = TIMER_INTENSET_COMPARE0_Msk;

    /* Enable timer event interrupt */
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    NVIC_SetPriority(TIMER2_IRQn, 3);
    NVIC_EnableIRQ(TIMER2_IRQn);
  }
  else
  {
    /* Need to do this just to make sure that it was not enabled before */
    NRF_TIMER2->INTENCLR = TIMER_INTENSET_COMPARE0_Msk;
    NVIC_DisableIRQ(TIMER2_IRQn);
  }
}

void test_timer2_start(uint32_t time)
{
  NRF_TIMER2->TASKS_CLEAR = 1;

  NRF_TIMER2->CC[0] = time;
  NRF_TIMER2->EVENTS_COMPARE[0] = 0;

  NRF_TIMER2->TASKS_START = 1;
}

void test_timer2_stop()
{
  NRF_TIMER2->TASKS_STOP = 1;
}

void test_timer2_set_count(uint32_t timer_count)
{
  if (timer_count == 0)
  {
    m_timer_count = 0;
    m_timeout = true;
  }
  else
  {
    m_timer_count = timer_count;
    m_timeout = false;
  }
}

uint32_t test_timer2_get_count(void)
{
  return m_timer_count;
}

bool test_timer2_get_timeout(void)
{
  return m_timeout;
}

uint32_t test_timer2_capture(void)
{
  NRF_TIMER2->TASKS_CAPTURE[1] = 1;
  return NRF_TIMER2->CC[1];
}

void TIMER2_IRQHandler(void)
{
  if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
  {
    /* Clear event */
    NRF_TIMER2->EVENTS_COMPARE[0] = 0;

    m_timer_count--;
    if (m_timer_count == 0 )
    {
      m_timeout = true;
      test_timer2_stop();
    }
  }

  /* Handle unexpected interrupts */
  if ((NRF_TIMER2->EVENTS_COMPARE[1] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0))
  {
    NRF_TIMER2->EVENTS_COMPARE[1] = 0;
  }
  if ((NRF_TIMER2->EVENTS_COMPARE[2] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE2_Msk) != 0))
  {
    NRF_TIMER2->EVENTS_COMPARE[2] = 0;
  }
  if ((NRF_TIMER2->EVENTS_COMPARE[3] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE3_Msk) != 0))
  {
    NRF_TIMER2->EVENTS_COMPARE[3] = 0;
  }
}


/**
  @}
*/
