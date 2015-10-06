#ifndef TEST_ASSERT_H__
#define TEST_ASSERT_H__

#include <stdint.h>
#include <stdbool.h>

#define ASSERT_TYPE_NORM      1
#define ASSERT_TYPE_EQ8       2
#define ASSERT_TYPE_EQ16      3
#define ASSERT_TYPE_EQ32      4
#define ASSERT_TYPE_STK       5
#define ASSERT_TEST_PAGE_NUM  ((NRF_FICR->CODESIZE) - 2)
#define ASSERT_STACK_PAGE_NUM ((NRF_FICR->CODESIZE) - 1)

#define ASSERT(expr) \
/*lint -e506 -e774 Constant value Boolean, Boolean within 'if' always evaluates to True */ \
{ \
uint8_t c1=(expr);\
if(c1)\
{\
  ;\
}\
else\
{\
  test_assert_handler(__LINE__, ASSERT_TYPE_NORM, 0, 0); \
}\
}

#define ASSERT_EQ8(v1,v2) \
/*lint -e506 -e774 Constant value Boolean, Boolean within 'if' always evaluates to True */ \
{ \
uint8_t c1=(v1);\
uint8_t c2=(v2);\
if(c1==c2)\
{\
  ;\
}\
else\
{\
  test_assert_handler(__LINE__, ASSERT_TYPE_EQ8, c1, c2); \
}\
}

#define ASSERT_EQ16(v1,v2) \
/*lint -e506 -e774 Constant value Boolean, Boolean within 'if' always evaluates to True */ \
{ \
uint16_t c1=(v1);\
uint16_t c2=(v2);\
if(c1==c2)\
{\
  ;\
}\
else\
{\
  test_assert_handler(__LINE__, ASSERT_TYPE_EQ16, c1, c2); \
}\
}

#define ASSERT_EQ32(v1,v2) \
/*lint -e506 -e774 Constant value Boolean, Boolean within 'if' always evaluates to True */ \
{ \
uint32_t c1=(v1);\
uint32_t c2=(v2);\
if(c1==c2)\
{\
  ;\
}\
else\
{\
  test_assert_handler(__LINE__, ASSERT_TYPE_EQ32, c1, c2); \
}\
}

/* Erase the pages we need for assert storage if they aren't already erased */
void test_assert_init(void);

/* Logs assert by recording line number and type. In equality asserts, compared elements are provided as c1 and c2 */
void test_assert_handler(uint16_t line_num, uint16_t type, uint32_t c1, uint32_t c2);

/* Logs a stack assert. Used as callback for softdevice_enable. */
void test_assert_stk_assert_handler(uint32_t pc, uint16_t line_number, const uint8_t * file_name);

#endif //TEST_ASSERT_H__
