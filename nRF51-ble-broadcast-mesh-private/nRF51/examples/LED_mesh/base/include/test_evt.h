#ifndef TEST_EVT_H__
#define TEST_EVT_H__

#include <stdint.h>
#include "ble.h"

#define TEST_EVENT_ANY BLE_EVT_INVALID
#define TEST_EVT_GET_NONBLOCKING 0
#define TEST_EVT_GET_EX 0xFFFFFFFE
#define TEST_EVT_GET_BLOCKING 0xFFFFFFFF

/* Assigns an event dispatcher (or NULL) to the automatic event handler */
uint32_t test_evt_init(void (*evt_dispatcher)(ble_evt_t*));

/* Runs for timeout_ms milliseconds until expected_event is found. Then calls dispatcher with pointer to event */
uint32_t test_evt_get(uint16_t expected_event, uint32_t timeout_ms, uint8_t *p_evt_buf, uint16_t evt_buf_len);

#endif //TEST_EVT_H__
