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
    Implementation of a direct sync mechanism using GPIO pins only.
  @}

  @defgroup test_pin_sync TEST_PIN_SYNC
  @{
*/

/* Header guard */
#ifndef TEST_PIN_SYNC_H__
#define TEST_PIN_SYNC_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"

// 'sync overseer'    'sync slave(s)'
//       slave0 +-------  master
//      |||||||+--------  slave1
//      ||||||+---------  slave2
//      |||||+----------  slave3
//      ||||+-----------  slave4
//      |||+------------  slave5
//      ||+-------------  slave6
//      |+--------------  slave7
//      +---------------  slave8

#define PIN_SYNC_FINISH_1  true             // Side to exit the sync point first
#define PIN_SYNC_FINISH_2  false            // Side to exit the sync point second
#ifdef NRF51
#define PIN_SYNC_P0       (1UL << (1 +  0)) // Device X pin 0 as seen from 'sync slave'
#define PIN_SYNC_P1       (1UL << (0 +  0)) // Device X pin 1 as seen from 'sync slave'
#endif
#ifdef NRF52
#define PIN_SYNC_P0       (1UL << (31 + 0)) // Device X pin 0 as seen from 'sync slave'
#define PIN_SYNC_P1       (1UL << (30 + 0)) // Device X pin 1 as seen from 'sync slave'
#endif
#define PIN_SYNC_D0P0     (1UL << (0 +  0)) // Device 0 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D0P1     (1UL << (1 +  0)) // Device 0 pin 1 as seen from 'sync overseer'
#define PIN_SYNC_D1P0     (1UL << (2 +  0)) // Device 1 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D1P1     (1UL << (3 +  0)) // Device 1 pin 1 as seen from 'sync overseer'
#define PIN_SYNC_D2P0     (1UL << (4 +  0)) // Device 2 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D2P1     (1UL << (5 +  0)) // Device 2 pin 1 as seen from 'sync overseer'
#define PIN_SYNC_D3P0     (1UL << (6 +  0)) // Device 3 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D3P1     (1UL << (7 +  0)) // Device 3 pin 1 as seen from 'sync overseer'
//                               0-1 +  8   // UART
//                               2-3 +  8   // UART
//                               4-5 +  8   // spare
//                               6-7 +  8   // spare
#define PIN_SYNC_D4P0     (1UL << (0 + 16)) // Device 4 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D4P1     (1UL << (1 + 16)) // Device 4 pin 1 as seen from 'sync overseer'
//                                 2 + 16   // LED1
//                                 3 + 16   // LED2
#define PIN_SYNC_D5P0     (1UL << (4 + 16)) // Device 5 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D5P1     (1UL << (5 + 16)) // Device 5 pin 1 as seen from 'sync overseer'
#define PIN_SYNC_D6P0     (1UL << (6 + 16)) // Device 6 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D6P1     (1UL << (7 + 16)) // Device 6 pin 1 as seen from 'sync overseer'
#define PIN_SYNC_D7P0     (1UL << (0 + 24)) // Device 7 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D7P1     (1UL << (1 + 24)) // Device 7 pin 1 as seen from 'sync overseer'
//                                 2 + 24   // 32kHz external osc
//                                 3 + 24   // 32kHz external osc
#define PIN_SYNC_D8P0     (1UL << (4 + 24)) // Device 8 pin 0 as seen from 'sync overseer'
#define PIN_SYNC_D8P1     (1UL << (5 + 24)) // Device 8 pin 1 as seen from 'sync overseer'
//                               6-7 + 24   // spare


#define PIN_SYNC_UPPER       PIN_SYNC_D0P0
#define PIN_SYNC_LOWER1      PIN_SYNC_D1P0
#define PIN_SYNC_LOWER2      PIN_SYNC_D2P0
#define PIN_SYNC_LOWER3      PIN_SYNC_D3P0
#define PIN_SYNC_LOWER4      PIN_SYNC_D4P0
#define PIN_SYNC_LOWER5      PIN_SYNC_D5P0
#define PIN_SYNC_LOWER6      PIN_SYNC_D6P0
#define PIN_SYNC_LOWER7      PIN_SYNC_D7P0
#define PIN_SYNC_LOWER8      PIN_SYNC_D8P0

static __inline void test_pin_sync_wait(uint32_t pin_mask, bool on_pull_up)
{
  uint32_t match_state = on_pull_up ? pin_mask : 0;

  while (true)
  {
    uint32_t pins_state = NRF_GPIO->IN;
    if ((pins_state & pin_mask) == match_state)
    {
      break;
    }
  }
}

static inline void test_pin_sync(bool finish_first, uint32_t out_mask, uint32_t in_mask)
{
  uint8_t i;
  for (i=0; i < 32; i++)
  {
    if ((out_mask & (1 << i)) != 0)
    {
      NRF_GPIO->PIN_CNF[i] =
            (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
          | (GPIO_PIN_CNF_DRIVE_S0S1     << GPIO_PIN_CNF_DRIVE_Pos)
          | (GPIO_PIN_CNF_PULL_Disabled  << GPIO_PIN_CNF_PULL_Pos)
          | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
          | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);
      NRF_GPIO->OUTCLR =    ((uint32_t)1 << (i));
    }
    if ((in_mask & (1 << i)) != 0)
    {
      NRF_GPIO->PIN_CNF[i] =
            (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
          | (GPIO_PIN_CNF_DRIVE_S0S1     << GPIO_PIN_CNF_DRIVE_Pos)
          | (GPIO_PIN_CNF_PULL_Disabled  << GPIO_PIN_CNF_PULL_Pos)
          | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
          | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);
    }
  }

  if (finish_first)
  {
    test_pin_sync_wait(in_mask, false);
    NRF_GPIO->DIRSET = out_mask;
    test_pin_sync_wait(in_mask, true);
    NRF_GPIO->DIRCLR = out_mask;
  }
  else
  {
    NRF_GPIO->DIRSET = out_mask;
    test_pin_sync_wait(in_mask, false);
    NRF_GPIO->DIRCLR = out_mask;
    test_pin_sync_wait(in_mask, true);
  }
}

static inline void test_pin_sync_point()
{
  test_pin_sync(PIN_SYNC_FINISH_1, PIN_SYNC_P0, PIN_SYNC_P1);
}

static inline void test_pin_sync_overseer(uint32_t device_mask)
{
  test_pin_sync(PIN_SYNC_FINISH_2, device_mask, device_mask << 1);
}

#endif // TEST_PIN_SYNC_H__

/**
  @}
*/
