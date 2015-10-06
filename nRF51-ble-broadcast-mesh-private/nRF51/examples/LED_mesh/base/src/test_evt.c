#include <stdint.h>
#include <string.h>

#include "ble.h"
#include "nrf_sdm.h"

#include "test_evt.h"
#include "test_rtc1.h"

/* Default callback is NULL */
static void (*p_evt_handler)(ble_evt_t * evt) = NULL;

uint32_t test_evt_init(void (*evt_dispatcher)(ble_evt_t*))
{
  p_evt_handler = evt_dispatcher; /* @TODO: Verify that this callback is within the app memory range */
  return NRF_SUCCESS;
}


uint32_t test_evt_get(uint16_t expected_event, uint32_t timeout_ms, uint8_t *p_evt_buf, uint16_t evt_buf_len)
{
  uint16_t evt_len;
  ble_evt_t *p_ble_evt = (ble_evt_t *) p_evt_buf;
  uint32_t errcode;

  if ((timeout_ms != TEST_EVT_GET_NONBLOCKING)
      && (timeout_ms != TEST_EVT_GET_BLOCKING)
      && (timeout_ms != TEST_EVT_GET_EX))
  {
    test_rtc1_cc2_timeout_set_ms(timeout_ms);
  }

  do {
      evt_len = evt_buf_len;
      errcode = sd_ble_evt_get(p_evt_buf, &evt_len);

      if (errcode == NRF_SUCCESS)
      {
        if (p_evt_handler != NULL)
        {
          p_evt_handler(p_ble_evt); /* Dispatch event to test logic first */
        }
      }

      if ( (errcode == NRF_SUCCESS) && ((expected_event == p_ble_evt->header.evt_id) || (expected_event == TEST_EVENT_ANY)))
      {
        return NRF_SUCCESS;
      }

      if (timeout_ms == TEST_EVT_GET_NONBLOCKING) /* No event received, abort */
      {
        if((expected_event == p_ble_evt->header.evt_id) || (expected_event == TEST_EVENT_ANY))
        {
          return errcode;
        } 
        else 
        {
          return NRF_ERROR_NOT_FOUND;
        }
      }

      if (NRF_ERROR_NOT_FOUND == errcode) /* No event received, continue */
      {
        sd_app_evt_wait();
        sd_nvic_ClearPendingIRQ(SD_EVT_IRQn);
        continue;
      }

      if (errcode != NRF_SUCCESS) /* and != NRF_ERROR_NOT_FOUND */
      {
          return errcode;
      }

    } while (!test_rtc1_cc2_timeout() || (timeout_ms == TEST_EVT_GET_BLOCKING));

 return NRF_ERROR_NOT_FOUND;
}
