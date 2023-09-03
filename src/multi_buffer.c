#include "multi_buffer.h"
#include "atomic_arm7m.h"

static void CountInc(volatile uint16_t *val)
{
    val[0]++;
}

static void CountDec(volatile uint16_t *val)
{
    val[0]--;
}

int MultiBufferPush(MultiBuffer_t *mulitBuffer, uint32_t length)
{
    if(mulitBuffer->count + 1 >= mulitBuffer->buffNum) //只有一个back可用,不push
        return -1;

    mulitBuffer->lenTable[mulitBuffer->back] = length;
    mulitBuffer->back = (mulitBuffer->back + 1) % mulitBuffer->buffNum;

    AtomicOnHaflWord(&mulitBuffer->count, CountInc);

    return 0;
}

int MultiBufferPop(MultiBuffer_t *mulitBuffer)
{
    if(mulitBuffer->count == 0)
        return -1;

    mulitBuffer->front = (mulitBuffer->front + 1) % mulitBuffer->buffNum;

    AtomicOnHaflWord(&mulitBuffer->count, CountDec);
    
    return 0;
}

int MultiBufferPeek(MultiBuffer_t *mulitBuffer, uint16_t index, uint8_t **ppbuff, uint32_t* pLength)
{
    if(mulitBuffer->count == 0 || index >= mulitBuffer->count )
        return -1;

    index = (mulitBuffer->front + index) % mulitBuffer->buffNum;
    ppbuff[0] = mulitBuffer->memAddr + index * mulitBuffer->buffSize;
    pLength[0] = mulitBuffer->lenTable[index];

    return 0;
}

int MultiBufferGetBack(MultiBuffer_t* mulitBuffer, uint8_t** ppbuff)
{
    // if(mulitBuffer->count >= mulitBuffer->buffNum)
    //     return -1;

    uint16_t index = mulitBuffer->back;
    ppbuff[0] = mulitBuffer->memAddr + index * mulitBuffer->buffSize;

    return 0;
}



