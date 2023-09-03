#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "cmsis_compiler.h"

#define MUTEXT_TRY_MAX_TIMES (3)

typedef volatile uint8_t MutexArm7m_t;

static inline void MutexArm7mInit(MutexArm7m_t *mutex)
{
    mutex[0] = 0;
}

static inline bool MutexArm7mTake(MutexArm7m_t *mutex)
{
    uint8_t tryTimes = 0;
    uint8_t val;

try_take:
    val = __LDREXB((volatile uint8_t *)mutex);
    if (val)
        return false;

    val = 1;
    if (__STREXB(val, (volatile uint8_t *)mutex))
    {
        if (++tryTimes < MUTEXT_TRY_MAX_TIMES)
            goto try_take;

        return false;
    }
    else
    {
        return true;
    }
}

static inline void MutexArm7mGive(MutexArm7m_t *mutex)
{
    mutex[0] = 0;
}

static inline bool MutexArm7mAvailable(MutexArm7m_t *mutex)
{
    return mutex[0] == 0;
}
