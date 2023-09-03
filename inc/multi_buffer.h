#pragma once

#include "cl_common.h"

typedef struct
{
    uint16_t front;
    uint16_t back;
    uint16_t count;

    uint8_t *memAddr;
    uint32_t buffSize;
    uint16_t buffNum;

    uint32_t *lenTable;
} MultiBuffer_t;

#define MULTIBUFFER_STATIC_DEF(name, buffSize, buffNum, modifier) \
    static uint8_t name##_static_mem[buffSize * buffNum] = {0};         \
    static uint32_t name##_length_table[buffNum] = {0};                 \
    modifier MultiBuffer_t name = {0, 0, 0, name##_static_mem, buffSize, buffNum, name##_length_table};

CL_Result_t MultiBufferPush(MultiBuffer_t *mulitBuffer, uint32_t length);
CL_Result_t MultiBufferPop(MultiBuffer_t *mulitBuffer);

//index from 0 to (count-1)
CL_Result_t MultiBufferPeek(MultiBuffer_t *mulitBuffer, uint16_t index, uint8_t **ppbuff, uint32_t* pLength);
CL_Result_t MultiBufferGetBack(MultiBuffer_t *mulitBuffer, uint8_t **ppbuff);

static inline uint16_t MultiBufferGetCount(MultiBuffer_t *mulitBuffer)
{
    return mulitBuffer->count;
}
