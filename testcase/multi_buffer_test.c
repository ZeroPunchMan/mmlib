#include "multi_buffer.h"
#include "assert.h"
#include "cmsis_compiler.h"
#include "cl_log.h"

#define TEST_BUFF_SIZE (256)
#define TEST_BUFF_NUM (12)

#define START_VAL (0)
#define PUSH_THRESH  (5)
#define POP_THRESH  (6)

MULTIBUFFER_STATIC_DEF(testBuffer, TEST_BUFF_SIZE, TEST_BUFF_NUM, static);

void MultiBufferTestInit(void)
{
}

static int PushBuff(void)
{
    static uint8_t start = START_VAL;

    uint8_t *pBuff;
    uint32_t buffLen = (start + 100) % (TEST_BUFF_SIZE / 2) + (TEST_BUFF_SIZE / 2);
    int res = MultiBufferGetBack(&testBuffer, &pBuff);
    // if (res != 0)
    //     return -1;

    for (uint32_t i = 0; i < buffLen; i++)
    {
        pBuff[i] = (uint8_t)(start + i);
    }

    res = MultiBufferPush(&testBuffer, buffLen);
    if (res != 0)
        return 1;

    start++;

    // CL_LOG_LINE("add:%d,%d", start, buffLen);
    return 0;
}

static int PopAndCheckBuff(void)
{
    static uint8_t start = START_VAL;

    int res = MultiBufferGetCount(&testBuffer);
    if (res == 0)
        return 1;

    uint8_t *pBuff;
    uint32_t buffLen;
    res = MultiBufferPeek(&testBuffer, 0, &pBuff, &buffLen);
    if (res != 0)
        return -1;

    if (pBuff[0] != start)
        return -1;

    if (buffLen != ((pBuff[0] + 100) % (TEST_BUFF_SIZE / 2) + (TEST_BUFF_SIZE / 2)))
        return -1;

    for (uint32_t i = 0; i < buffLen; i++)
    {
        if (pBuff[i] != (uint8_t)(pBuff[0] + i))
            return -1;
    }

    res = MultiBufferPop(&testBuffer);
    if (res != 0)
        return -1;

    // CL_LOG_LINE("add:%d,%d", start, buffLen);
    start++;

    return 0;
}

void MultiBufferTestPush(void)
{
    if (MultiBufferGetCount(&testBuffer) <= PUSH_THRESH)
    {
        int res = PushBuff();
        if (res < 0)
            __BKPT(1);
    }
}

void MultiBufferTestPop(void)
{
    // static uint8_t count = 0;
    // if (++count < 20)
    //     return;

    // count = 0;
    if (MultiBufferGetCount(&testBuffer) >= POP_THRESH)
    {
        int res = PopAndCheckBuff();
        if (res < 0)
            __BKPT(1);
    }
}
