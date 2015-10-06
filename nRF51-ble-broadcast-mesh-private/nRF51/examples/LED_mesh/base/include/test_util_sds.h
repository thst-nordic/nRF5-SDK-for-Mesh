#ifndef THROUGHPUT_REQ_SDS_H_
#define THROUGHPUT_REQ_SDS_H_

#define RUN_ALL_PERF 0
#define STATIC_BANDWIDTH 1

#define GATT_PAYLOAD_LEN_BYTES 20
#define L2CAP_PAYLOAD_LEN_BYTES 23

#define TOTAL_EXPECTED_TX_BUFFERS_SLAVE  7
#define TOTAL_EXPECTED_TX_BUFFERS_MASTER  4

#define CENTRAL_TX_PACKS_CONN_EVT 3
#define CENTRAL_RX_PACKS_CONN_EVT 3
#define CENTRAL_WREQ_PACKS_CONN_EVT 0.5

#define PERIPH_TX_PACKS_CONN_EVT (TOTAL_EXPECTED_TX_BUFFERS_SLAVE -1)
#define PERIPH_RX_PACKS_CONN_EVT (TOTAL_EXPECTED_TX_BUFFERS_SLAVE -1)
#define PERIPH_WREQ_PACKS_CONN_EVT 0.5

#define TX_PACKS_CONN_EVT         PERIPH_TX_PACKS_CONN_EVT
#define RX_PACKS_CONN_EVT         PERIPH_RX_PACKS_CONN_EVT
#define WREQ_PACKS_CONN_EVT       PERIPH_WREQ_PACKS_CONN_EVT
#define TOTAL_EXPECTED_PERIPH_TX_BUFFERS TOTAL_EXPECTED_TX_BUFFERS_SLAVE


/*note: the numbers below are temptative values at 1 packet transmitted and received per conn interval 
  bps = packets_per_sec (1000 / ci) * bytes_per_packet (always 20) * 8
  example for ci = 20ms, WR_CMD (no response) : bps = 1000 / 20 * 20 * 8 = 8000. 
  Our threshold, 7800 bps, gives 200 bps margin to account for transmission errors.
*/

#define PERF_FACTOR 0.9
#define PERF_BPS(CI_MS, PPCE, BPP) ((1000.0 / CI_MS)  /* connection intervals per second */ \
                                    * PPCE /* packets per connection event */ \
                                    * BPP /* bytes per packet */ \
                                    * 8 /* bits per byte */)

#define PERF_REQ_BPS(CI_MS, PPCE, BPP) (unsigned int)(((PERF_FACTOR * PERF_BPS(CI_MS, PPCE, BPP))))

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define CI_TO_MS(CI_MS) (CI_MS * 1.25)


/*PERIPHERAL PERFORMANE CRITERIA*/

#ifdef NRF51
#define PROCESSOR_LIMIT 0.83
#endif
#ifdef NRF52
#define PROCESSOR_LIMIT 1.0
#endif
/* TEST LIMITS - ENC ON  -  FULL BUFFERS */
#define GATTC_RX_HVN_REQ_BPS      (uint32_t) PERF_REQ_BPS(7.5,     RX_PACKS_CONN_EVT                       , GATT_PAYLOAD_LEN_BYTES )
#define GATTC_TX_WCMD_REQ_BPS     (uint32_t) PERF_REQ_BPS(7.5,     (TX_PACKS_CONN_EVT)                     , GATT_PAYLOAD_LEN_BYTES )
#define GATTC_TX_WREQ_REQ_BPS     (uint32_t) PERF_REQ_BPS(7.5,     WREQ_PACKS_CONN_EVT                     , GATT_PAYLOAD_LEN_BYTES )
#define GATTC_RX_HVN_TX_REQ_BPS   (uint32_t) PERF_REQ_BPS(7.5,     (TX_PACKS_CONN_EVT)                     , GATT_PAYLOAD_LEN_BYTES )
#define GATTS_TX_HVN_REQ_BPS      (uint32_t) PERF_REQ_BPS(7.5,     (TX_PACKS_CONN_EVT)                     , GATT_PAYLOAD_LEN_BYTES )
#define GATTS_RX_WCMD_REQ_BPS     (uint32_t) PERF_REQ_BPS(7.5,     RX_PACKS_CONN_EVT                       , GATT_PAYLOAD_LEN_BYTES )
#define GATTS_TX_WREQ_REQ_BPS     (uint32_t) PERF_REQ_BPS(7.5,     WREQ_PACKS_CONN_EVT                     , GATT_PAYLOAD_LEN_BYTES )
#define GATTS_TX_HVN_TX_REQ_BPS   (uint32_t)(PERF_REQ_BPS(7.5,     (TX_PACKS_CONN_EVT)                     , GATT_PAYLOAD_LEN_BYTES ) * PROCESSOR_LIMIT)
#define L2CAP_RX_HVN_REQ_BPS      (uint32_t) PERF_REQ_BPS(7.5,     RX_PACKS_CONN_EVT                       , L2CAP_PAYLOAD_LEN_BYTES)
#define L2CAP_TX_HVN_REQ_BPS      (uint32_t) PERF_REQ_BPS(7.5,     (TX_PACKS_CONN_EVT)                     , L2CAP_PAYLOAD_LEN_BYTES)
#define L2CAP_RX_TX_REQ_BPS       (uint32_t) PERF_REQ_BPS(7.5,     RX_PACKS_CONN_EVT                       , L2CAP_PAYLOAD_LEN_BYTES)



#if RUN_ALL_PERF
  #define PERF_PACKET_COUNT1 32578
  #define PERF_PACKET_COUNT2 8192
#else
  #define PERF_PACKET_COUNT1 4096
  #define PERF_PACKET_COUNT2 1024
#endif



/*MULTILINK (CENTRAL & BOTH) PERFORMANE CRITERIA*/


#define  PERF_SLAVES_MAX  3
#define  PERF_MASTERS_MAX 1


#define BPS_CEN_THRSH_7P5MS           (uint32_t)  PERF_REQ_BPS(7.5,      CENTRAL_TX_PACKS_CONN_EVT               , GATT_PAYLOAD_LEN_BYTES )
#define BPS_CEN_THRSH_20MS            (uint32_t)  PERF_REQ_BPS( 20,      CENTRAL_TX_PACKS_CONN_EVT               , GATT_PAYLOAD_LEN_BYTES )
#define BPS_CEN_THRSH_50MS            (uint32_t)  PERF_REQ_BPS( 50,      CENTRAL_TX_PACKS_CONN_EVT               , GATT_PAYLOAD_LEN_BYTES )

#define BPS_PER_THRSH_7P5MS           (uint32_t)  PERF_REQ_BPS(7.5,      PERIPH_TX_PACKS_CONN_EVT                , GATT_PAYLOAD_LEN_BYTES )
#define BPS_PER_THRSH_20MS            (uint32_t)  PERF_REQ_BPS( 20,      PERIPH_TX_PACKS_CONN_EVT                , GATT_PAYLOAD_LEN_BYTES )
#define BPS_PER_THRSH_50MS            (uint32_t)  PERF_REQ_BPS( 50,      PERIPH_TX_PACKS_CONN_EVT                , GATT_PAYLOAD_LEN_BYTES )

#define PERF_CON_FACTOR 0.8

#define BPS_CON_THRSH_7P5MS           (uint32_t) (PERF_REQ_BPS(7.5,      CENTRAL_TX_PACKS_CONN_EVT                , GATT_PAYLOAD_LEN_BYTES ) * PERF_CON_FACTOR)
#define BPS_CON_THRSH_20MS            (uint32_t) (PERF_REQ_BPS( 20,      CENTRAL_TX_PACKS_CONN_EVT                , GATT_PAYLOAD_LEN_BYTES ) * PERF_CON_FACTOR)
#define BPS_CON_THRSH_50MS            (uint32_t) (PERF_REQ_BPS( 50,      CENTRAL_TX_PACKS_CONN_EVT                , GATT_PAYLOAD_LEN_BYTES ) * PERF_CON_FACTOR)


#if RUN_ALL_PERF
  #define PERF_TEST_CONFIGS  {{6,2},{3,1},{,1}}
  #define PERF_PACKET_COUNT3 2048
  #define PERF_PACKET_COUNT4 1024
  #define PERF_PACKET_COUNT5 512
#else
  #define PERF_TEST_CONFIGS_NUM 7
  #define PERF_TEST_CONFIGS  {{3,1},{3,0},{0,1},{3,1},{0,1},{3,0},{3,1}}  ///
  #define PERF_PACKET_COUNT3 0x40  //high
  #define PERF_PACKET_COUNT4 0x30  //medium
  #define PERF_PACKET_COUNT5 0x20  //low
  //#define PERF_TEST_BANDWIDTH  {{{5,5,5},{10}}}     // 5kpbs for each slave, 10kbps for the single master connection.
#endif


void test_util_central_throughput_sds_check(void);
void test_util_peripheral_throughput_sds_check(void);
void test_util_concurrent_throughput_sds_check(void); 
#endif //THROUGHPUT_REQ_SDS_H_


