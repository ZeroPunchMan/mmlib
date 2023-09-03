#include "mutex_arm7m.h"
#include "stdbool.h"

#define ENABLE_MUTEX    (1)

#define MUTEX_COUNT_TIMES  (50000UL)
static volatile int32_t mutexTestCounter = 0;
static MutexArm7m_t testMutex;
static volatile bool mutexTestDone = false;

void MutexTestInit(void)
{
    MutexArm7mInit(&testMutex);
}

void MutexTestMain(void)
{

    while (1)
    {
        static uint32_t incCount = 0;

#if ENABLE_MUTEX
        if (MutexArm7mTake(&testMutex))
#endif
        {
            if (incCount < MUTEX_COUNT_TIMES + 100)
            {
                incCount++;
                mutexTestCounter++;
                for (int i = 0; i < 100; i++)
                {
                    mutexTestCounter++;
                    mutexTestCounter--;
                }
            }
            else
            {
#if ENABLE_MUTEX
                MutexArm7mGive((&testMutex));
#endif
                break;
            }

#if ENABLE_MUTEX
            MutexArm7mGive((&testMutex));
#endif
        }
    }
    while (1)
    {
        if (mutexTestDone)
        {
            __BKPT(1);
        }
    }
}

void MutexTestIsr(void)
{
    static uint32_t incCount = 0;

#if ENABLE_MUTEX
    if (MutexArm7mTake(&testMutex))
#endif
    {
        if (incCount < MUTEX_COUNT_TIMES)
        {
            incCount++;
            mutexTestCounter--;
        }
        else
        {
            mutexTestDone = true;
        }

#if ENABLE_MUTEX
        MutexArm7mGive((&testMutex));
#endif
    }
}
