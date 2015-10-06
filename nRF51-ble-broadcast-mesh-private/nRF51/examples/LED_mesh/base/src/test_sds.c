#include "test_util_sds.h"
//#include "nrf_cunit.h"
#include "test_console.h"
#include "test_util.h"

#define SDS_PRINT true

#define SDS_GATTC_RX_HVN_REQ_BPS           115200
#define SDS_GATTC_TX_WCMD_REQ_BPS          115200
#define SDS_GATTC_TX_WREQ_REQ_BPS            9600
#define SDS_GATTC_RX_HVN_TX_REQ_BPS        115200
#define SDS_GATTS_TX_HVN_REQ_BPS           115200
#define SDS_GATTS_RX_WCMD_REQ_BPS          115200
#define SDS_GATTS_TX_WREQ_REQ_BPS            9600
#define SDS_GATTS_TX_HVN_TX_REQ_BPS        115200
#define SDS_GATTS_TX_HVN_TX_REQ_BPS_NRF51   94000
#define SDS_L2CAP_RX_HVN_REQ_BPS           132480
#define SDS_L2CAP_TX_HVN_REQ_BPS           132480
#define SDS_L2CAP_RX_TX_REQ_BPS            132480

#define SDS_BPS_CEN_THRSH_7P5MS             57600
#define SDS_BPS_CEN_THRSH_20MS              21600
#define SDS_BPS_CEN_THRSH_50MS               8640

#define SDS_BPS_CON_THRSH_7P5MS            (57600 * PERF_CON_FACTOR)
#define SDS_BPS_CON_THRSH_20MS             (21600 * PERF_CON_FACTOR)
#define SDS_BPS_CON_THRSH_50MS             ( 8640 * PERF_CON_FACTOR)

/*
#define SDS_BPS_CEN_THRSH_7P5MS             19200
#define SDS_BPS_CEN_THRSH_20MS               7200
#define SDS_BPS_CEN_THRSH_50MS               2880

#define SDS_BPS_CON_THRSH_7P5MS             (19200 * PERF_CON_FACTOR)
#define SDS_BPS_CON_THRSH_20MS              ( 7200 * PERF_CON_FACTOR)
#define SDS_BPS_CON_THRSH_50MS              ( 2880 * PERF_CON_FACTOR)
*/

void test_util_concurrent_throughput_sds_check() 
{
  /* TEST LIMITS FROM s130 v2.alpha SDS Chapter 13 (BLE data throughput) */
  
  CU_ASSERT(BPS_CON_THRSH_7P5MS  == SDS_BPS_CON_THRSH_7P5MS );
  CU_ASSERT(BPS_CON_THRSH_20MS   == SDS_BPS_CON_THRSH_20MS  );
  CU_ASSERT(BPS_CON_THRSH_50MS   == SDS_BPS_CON_THRSH_50MS  );

  if (SDS_PRINT)
  {
    test_util_tag_open("CONCURRENT SDS_PRINT");
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("BPS_CON_THRSH_7P5MS = ", 18);
    test_console_put_dec32bit(BPS_CON_THRSH_7P5MS );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("BPS_CON_THRSH_20MS  = ", 18);
    test_console_put_dec32bit(BPS_CON_THRSH_20MS  );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("BPS_CON_THRSH_50MS  = ", 18);
    test_console_put_dec32bit(BPS_CON_THRSH_50MS    );
    test_console_put_chars("    \n:", 6);
    test_util_tag_close();
  }
}



void test_util_central_throughput_sds_check() 
{
  /* TEST LIMITS FROM s130 v2.alpha SDS Chapter 13 (BLE data throughput) */
  
  CU_ASSERT(BPS_CEN_THRSH_7P5MS  == SDS_BPS_CEN_THRSH_7P5MS );
  CU_ASSERT(BPS_CEN_THRSH_20MS   == SDS_BPS_CEN_THRSH_20MS  );
  CU_ASSERT(BPS_CEN_THRSH_50MS   == SDS_BPS_CEN_THRSH_50MS  );

  if (SDS_PRINT)
  {
    test_util_tag_open("CENTRAL SDS_PRINT");
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("BPS_CEN_THRSH_7P5MS = ", 18);
    test_console_put_dec32bit(BPS_CEN_THRSH_7P5MS );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("BPS_CEN_THRSH_20MS  = ", 18);
    test_console_put_dec32bit(BPS_CEN_THRSH_20MS  );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("BPS_CEN_THRSH_50MS  = ", 18);
    test_console_put_dec32bit(BPS_CEN_THRSH_50MS    );
    test_console_put_chars("    \n:", 6);
    test_util_tag_close();
  }
}

void test_util_peripheral_throughput_sds_check() 
{
  /* TEST LIMITS FROM s130 v2.alpha SDS Chapter 13 (BLE data throughput) */
  
  /*GATTC PERFORMANCE CRITERIA*/
  CU_ASSERT(GATTC_RX_HVN_REQ_BPS    == SDS_GATTC_RX_HVN_REQ_BPS );
  CU_ASSERT(GATTC_TX_WCMD_REQ_BPS   == SDS_GATTC_TX_WCMD_REQ_BPS);
  CU_ASSERT(GATTC_TX_WREQ_REQ_BPS   == SDS_GATTC_TX_WREQ_REQ_BPS);
  CU_ASSERT(GATTC_RX_HVN_TX_REQ_BPS == SDS_GATTC_RX_HVN_TX_REQ_BPS );

  /*GATTS PERFORMANCE CRITERIA*/
  CU_ASSERT(GATTS_TX_HVN_REQ_BPS    == SDS_GATTS_TX_HVN_REQ_BPS  );
  CU_ASSERT(GATTS_RX_WCMD_REQ_BPS   == SDS_GATTS_RX_WCMD_REQ_BPS );
  CU_ASSERT(GATTS_TX_WREQ_REQ_BPS   == SDS_GATTS_TX_WREQ_REQ_BPS );
#ifdef NRF51
  CU_ASSERT(GATTS_TX_HVN_TX_REQ_BPS >=  SDS_GATTS_TX_HVN_TX_REQ_BPS_NRF51 );
#endif
#ifdef NRF52
  CU_ASSERT(GATTS_TX_HVN_TX_REQ_BPS == SDS_GATTS_TX_HVN_TX_REQ_BPS );
#endif

  /*L2CAP PERFORMANCE CRITERIA*/
  CU_ASSERT(L2CAP_RX_HVN_REQ_BPS    == SDS_L2CAP_RX_HVN_REQ_BPS );
  CU_ASSERT(L2CAP_TX_HVN_REQ_BPS    == SDS_L2CAP_TX_HVN_REQ_BPS );
  CU_ASSERT(L2CAP_RX_TX_REQ_BPS     == SDS_L2CAP_RX_TX_REQ_BPS  );

  if (SDS_PRINT)
  {
    test_util_tag_open("PERIPHERAL SDS_PRINT");
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTC_RX_HVN_REQ_BPS  = ", 25);
    test_console_put_dec32bit(GATTC_RX_HVN_REQ_BPS  );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTC_TX_WCMD_REQ_BPS = ", 25);
    test_console_put_dec32bit(GATTC_TX_WCMD_REQ_BPS   );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTC_TX_WREQ_REQ_BPS = ", 25);
    test_console_put_dec32bit(GATTC_TX_WREQ_REQ_BPS   );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTC_RX_HVN_TX_REQ_BPS=", 25);
    test_console_put_dec32bit(GATTC_RX_HVN_TX_REQ_BPS );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTS_TX_HVN_REQ_BPS  = ", 25);
    test_console_put_dec32bit(GATTS_TX_HVN_REQ_BPS    );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTS_RX_WCMD_REQ_BPS = ", 25);
    test_console_put_dec32bit(GATTS_RX_WCMD_REQ_BPS   );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTS_TX_WREQ_REQ_BPS = ", 25);
    test_console_put_dec32bit(GATTS_TX_WREQ_REQ_BPS   );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("GATTS_TX_HVN_TX_REQ_BPS=", 25);
    test_console_put_dec32bit(GATTS_TX_HVN_TX_REQ_BPS );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("L2CAP_RX_HVN_REQ_BPS  = ", 25);
    test_console_put_dec32bit(L2CAP_RX_HVN_REQ_BPS    );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("L2CAP_TX_HVN_REQ_BPS  = ", 25);
    test_console_put_dec32bit(L2CAP_TX_HVN_REQ_BPS    );
    test_console_put_chars("    \n:", 6);
    test_console_put_chars("L2CAP_RX_TX_REQ_BPS   = ", 25);
    test_console_put_dec32bit(L2CAP_RX_TX_REQ_BPS     );
    test_console_put_chars("    \n:", 6);
    test_util_tag_close();
  }
}
