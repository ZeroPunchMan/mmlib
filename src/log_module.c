#include "log_module.h"
#include "cl_pool.h"
#include "cl_queue.h"
#include "stdio.h"
#include "stdarg.h"

#include "FreeRTOS.h"
#include "task.h"

//freertos中,优先级比临界区高的中断ISR里,不要打log
static inline uint32_t EnterCritical(void)
{
    if (xPortIsInsideInterrupt())
        return taskENTER_CRITICAL_FROM_ISR();
    else
        taskENTER_CRITICAL();

    return 0;
}

static inline void ExitCritical(uint32_t x)
{
    if (xPortIsInsideInterrupt())
        taskEXIT_CRITICAL_FROM_ISR(x);
    else
        taskEXIT_CRITICAL();
}

#include "usart.h"
static inline void SendLog(const uint8_t *buff, uint8_t len)
{

    Usart_Send(buff, len);
}

// 用内存块来缓存日志,超出则用..省略号结尾, 支持ISR中打印日志
typedef struct
{
    uint8_t buff[LOG_BLOCK_LEN];
    uint8_t len; // 打印日志,不需要结尾\0
} LogCell_t;

#define LOG_BLOCK_NUM (32) // 分配32个内存块
CL_POOL_DEFINE(logPool, LOG_BLOCK_NUM, LogCell_t, static);
CL_QUEUE_DEF_INIT(logQueue, LOG_BLOCK_NUM, uint32_t, static);

void LogModule_Init(void)
{
    CL_PoolInit(&logPool);
}

// static uint32_t logMissing = 0;
void LogModule_Process(void)
{
    LogCell_t *logCell;
    CL_Result_t res = CL_QueuePoll(&logQueue, &logCell);
    if (res == CL_ResSuccess)
    {
        SendLog(logCell->buff, logCell->len);

        uint32_t x = EnterCritical();
        CL_PoolFreeFast(&logPool, logCell);
        ExitCritical(x);
    }
}

void LogPrint(const char *fmt, ...)
{
    uint32_t x = EnterCritical();
    LogCell_t *logCell = CL_PoolAllocFast(&logPool, sizeof(LogCell_t));
    // if (logCell == NULL) // 空间不够,直接丢日志算了 // todo 空闲时打印出丢失的日志数量
    //     logMissing++;
    ExitCritical(x);

    if (logCell == NULL) // 空间不够,直接丢日志算了
        return;

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf((char *)(logCell->buff), LOG_BLOCK_LEN, fmt, args);
    va_end(args);

    if (len >= LOG_BLOCK_LEN)
    {
        logCell->buff[LOG_BLOCK_LEN - 3] = '.';
        logCell->buff[LOG_BLOCK_LEN - 2] = '.';
        logCell->buff[LOG_BLOCK_LEN - 1] = '\n';

        logCell->len = LOG_BLOCK_LEN;
    }
    else
    {
        logCell->len = len;
    }

    x = EnterCritical();
    CL_QueueAdd(&logQueue, &logCell);
    ExitCritical(x);
}
