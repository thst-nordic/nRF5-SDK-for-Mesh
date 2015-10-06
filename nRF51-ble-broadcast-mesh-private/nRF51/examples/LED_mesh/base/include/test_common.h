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
    Header file for defines, types and interfaces common to all test frameworks.
  @}

  @defgroup test_common TEST_COMMON
  @{
*/

/* Header guard */
#ifndef TEST_COMMON_H__
#define TEST_COMMON_H__

#include <stdint.h>
#include <stdbool.h>

/* ARM KEIL specific macros */
#if defined(__arm__) && !defined(_lint)
#  define PACKED __packed
#  define ALIGNED(n) __align(n)
#else
#  define PACKED
#  if defined(_WIN32)
#    define ALIGNED(n) __declspec(align(n))
#  else
#    define ALIGNED(n)
#  endif
#endif

#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))
#define NUMELTS(x)          (sizeof((x)) / sizeof(x[0]))
#define MSEC_TO_UNITS(TIME, RESOLUTION) (((TIME) * 1000) / (RESOLUTION))

#define TEST_DEFAULT_UUID_VS_COUNT      (10)
#define TEST_DEFAULT_PERIPH_CONN_COUNT  (1)
#define TEST_DEFAULT_CENTRAL_CONN_COUNT (3)
#define TEST_DEFAULT_CONN_COUNT         (TEST_DEFAULT_PERIPH_CONN_COUNT + TEST_DEFAULT_CENTRAL_CONN_COUNT)
#define TEST_DEFAULT_CENTRAL_SEC_COUNT  (1)
#define TEST_DEFAULT_L2CAP_TX_MPS       (23)
#define TEST_DEFAULT_L2CAP_RX_MPS       (23)
#define TEST_DEFAULT_SYSTEM_TX_COUNT              (6)
#define TEST_DEFAULT_CENTRAL_USER_TX_PACKET_COUNT (3 + 1)
#define TEST_DEFAULT_PERIPH_USER_TX_PACKET_COUNT  (6 + 1)
#define TEST_DEFAULT_CENTRAL_RX_PACKET_COUNT      (3)
#define TEST_DEFAULT_CENTRAL_TX_PACKET_COUNT      (TEST_DEFAULT_CENTRAL_USER_TX_PACKET_COUNT + TEST_DEFAULT_SYSTEM_TX_COUNT)
#define TEST_DEFAULT_PERIPH_RX_PACKET_COUNT       (6)
#define TEST_DEFAULT_PERIPH_TX_PACKET_COUNT       (TEST_DEFAULT_PERIPH_USER_TX_PACKET_COUNT  + TEST_DEFAULT_SYSTEM_TX_COUNT)
#define TEST_DEFAULT_MM_PDU_MEM_LEN     (0x858)
#ifdef ROLE_MASTER
  #define TEST_DEFAULT_TX_PACKET_COUNT TEST_DEFAULT_CENTRAL_TX_PACKET_COUNT
#endif
#ifdef ROLE_SLAVE
  #define TEST_DEFAULT_TX_PACKET_COUNT TEST_DEFAULT_PERIPH_TX_PACKET_COUNT
#endif

extern uint32_t Image$$RW_IRAM1$$Base; //lint !e27 Illegal character

/* The start address of the RAM area in which application access is allowed. */
#define TEST_USER_RAM_BEGIN   ((uint32_t) &Image$$RW_IRAM1$$Base) //lint !e27 Illegal character

enum time_unit_conversion_coefficients
{
  UNIT_0_625_MS = 625,
  UNIT_1_25_MS  = 1250,
  UNIT_10_MS    = 10000
};

#endif /* TEST_COMMON_H__ */

/**
  @}
*/
