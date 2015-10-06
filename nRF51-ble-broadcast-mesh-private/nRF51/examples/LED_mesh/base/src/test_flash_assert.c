#include <stdint.h>
#include <string.h>

#include "test_assert.h"
#include "nrf.h"

#define TEST_FLASH_PAGE_SIZE     ((uint16_t)NRF_FICR->CODEPAGESIZE)

static bool m_init_complete = false;
static uint32_t m_num_words_written = 0;

static void m_flash_page_erase(uint8_t page_num)
{
    uint32_t * p_page = (uint32_t*)(TEST_FLASH_PAGE_SIZE * page_num);

    /* Turn on flash erase enable and wait until the NVMC is ready. */
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Een << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

    /* Erase page. */
    NRF_NVMC->ERASEPAGE = (uint32_t)p_page;
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

    /* Turn off flash erase enable and wait until the NVMC is ready. */
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
}

static void m_flash_word_write(uint32_t* p_address, uint32_t* p_values, uint8_t word_count)
{   
    uint8_t i;

    /* Turn on flash write enable and wait until the NVMC is ready. */
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

    for(i=0; i < word_count; ++i)
    {
      *p_address = p_values[i];
      /* Wait flash write to finish */
      while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
      p_address += 1;
    }

    /* Turn off flash write enable and wait until the NVMC is ready. */
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
}

static void m_flash_page_read(uint8_t page_num, uint32_t* p_out_array, uint16_t word_count)
{
    uint32_t* p_curr_addr;
    uint16_t  i;

    p_curr_addr = (uint32_t*)(TEST_FLASH_PAGE_SIZE * page_num);
    for (i=0; i < word_count; i++)
    {
      p_out_array[i] = *p_curr_addr;
      p_curr_addr++;
    }
}

void test_assert_flash_enable(void) __attribute__ ((section ("test_assert_flash_enable")));
void test_assert_flash_enable() 
{
  /* Disable MPU protection so we can write to flash despite any API */
  #ifdef NRF51
  NRF_MPU->PERR0 &= ~MPU_PERR0_NVMC_Msk;
  #endif
}

void test_assert_init(void)
{
  uint32_t buffer[256] = {0};
  uint16_t i, page;
  bool     is_ff       = true;
  uint8_t  pages[2];

  pages[0] = ASSERT_TEST_PAGE_NUM;
  pages[1] = ASSERT_STACK_PAGE_NUM;

  test_assert_flash_enable();

  for(page = 0; page < sizeof(pages)/sizeof(pages[0]); page++)
  {
    is_ff = true;
    m_flash_page_read(pages[page], buffer, 256);
    for(i=0; i < 256; i++)
    {
      if(buffer[i] != 0xFF)
      {
        is_ff = false;
        break;
      }
    }
    if (!is_ff)
    {
      m_flash_page_erase(pages[page]);
    }    
  }
  m_init_complete = true;
}

void test_assert_handler(uint16_t line_num, uint16_t type, uint32_t c1, uint32_t c2)
{
  uint8_t word_size;
  uint32_t buffer[3];
  memset(buffer, 0xFF, sizeof(buffer));

  /* If there is no room for the assert, or the flashing has not been done, return */
  if (!m_init_complete || (m_num_words_written >= 252))
  {
    return;
  }
  
  buffer[0] = (type << 24) | (((line_num >> 8) & 0xFF) << 16) | ((line_num & 0xFF) << 8) | 0xFF;

  switch (type)
  {
    case ASSERT_TYPE_NORM:
      word_size = 1;
      break;

    case ASSERT_TYPE_EQ8:
      buffer[1] = ((c1 & 0xFF) << 24) | ((c2  & 0xFF)<< 16);
      word_size = 2;
      break;

    case ASSERT_TYPE_EQ16:
      buffer[1] = ((c1 & 0xFFFF) << 16) | ((c2  & 0xFFFF));
      word_size = 2;
    break;
    
    case ASSERT_TYPE_EQ32:
      buffer[1] = c1;
      buffer[2] = c2;
      word_size = 3;
    break;
  }

  m_flash_word_write((uint32_t *)(TEST_FLASH_PAGE_SIZE * ASSERT_TEST_PAGE_NUM) + m_num_words_written, buffer, word_size);
  m_num_words_written += word_size;
}

void test_assert_stk_assert_handler(uint32_t pc, uint16_t line_number, const uint8_t * file_name)
{
  uint8_t i, len;
  union
  {
    uint8_t  buff8[40];
    uint32_t buff32[10];
  } buffer;
  memset(buffer.buff8, 0xFF, sizeof(buffer));
  
  if (!m_init_complete)
  {
    while (true) /* Nowhere to write the stack assert. Just block forever. */
    {}
  }
  else
  {
    /* Write the file name to the buffer (including \0) */
    for (i=4; i<sizeof(buffer.buff8)-1; i++)
    {
      buffer.buff8[i] = file_name[i-4];
      if (buffer.buff8[i] == '\0')
      {
        break;
      }
    }
    buffer.buff8[i] = '\0';
    i++;
    
    /* Pad any missing bytes, word-aligning it */
    while(((i % 4) != 0) && (i < sizeof(buffer.buff8) - 1))
    {
      buffer.buff8[++i] = 0xFF;
    }

    /* Save the aligned length */
    len = i;
   
    buffer.buff32[0] = (ASSERT_TYPE_STK << 24) | (((line_number >> 8) & 0xFF) << 16) | ((line_number & 0xFF) << 8) | (len-4);
   
    /* Write the words we have */
    m_flash_word_write((uint32_t *)(TEST_FLASH_PAGE_SIZE * ASSERT_STACK_PAGE_NUM), buffer.buff32, (len/4));
  }
  
  while (true)
  {}
}
