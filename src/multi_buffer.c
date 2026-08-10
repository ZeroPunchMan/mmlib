#include "multi_buffer.h"

// 多页缓存
static inline uint16_t NextPos(uint16_t pos, uint16_t step, uint16_t max)
{
    pos = (pos + step) % (max);
    return pos;
}

// push和getback都是生产者用
CL_Result_t MultiBufferPush(MultiBuffer_t *mulitBuffer, uint32_t length)
{
    uint16_t nextPage = NextPos(mulitBuffer->back, 1, mulitBuffer->buffNum);
    if (nextPage == mulitBuffer->front) // back下一页就是front了,说明只有一个back可用,此时不push
        return CL_ResFailed;

    uint16_t slot = mulitBuffer->lenTable[mulitBuffer->back] = length;
    mulitBuffer->back = nextPage;

    return CL_ResSuccess;
}

CL_Result_t MultiBufferGetBack(MultiBuffer_t *mulitBuffer, uint8_t **ppbuff)
{
    // Push函数保证了只有一个back页时不push,所以直接拿当前back页就行
    uint16_t index = mulitBuffer->back;
    ppbuff[0] = mulitBuffer->memAddr + index * mulitBuffer->buffSize;

    return CL_ResSuccess;
}

// pop和peek都是消费者用
CL_Result_t MultiBufferPop(MultiBuffer_t *mulitBuffer)
{
    if (mulitBuffer->front == mulitBuffer->back)  //此时还没有push任何一个back页
        return CL_ResFailed;

    mulitBuffer->front = NextPos(mulitBuffer->front, 1, mulitBuffer->buffNum);

    return CL_ResSuccess;
}

CL_Result_t MultiBufferPeek(MultiBuffer_t *mulitBuffer, uint16_t index, uint8_t **ppbuff, uint32_t *pLength)
{
    if (index >= MultiBufferGetCount(mulitBuffer))
        return CL_ResFailed;

    index = (mulitBuffer->front + index) % mulitBuffer->buffNum;
    ppbuff[0] = mulitBuffer->memAddr + index * mulitBuffer->buffSize;
    pLength[0] = mulitBuffer->lenTable[index];

    return CL_ResSuccess;
}

uint16_t MultiBufferGetCount(MultiBuffer_t *mulitBuffer)
{
    uint16_t front, back;
    front = mulitBuffer->front;
    back = mulitBuffer->back;

    if (back >= front)
    {
        return back - front;
    }
    else
    {
        return mulitBuffer->buffNum - front + back;
    }
}
