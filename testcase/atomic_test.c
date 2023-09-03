#include "atomic_arm7m.h"
#include "stdbool.h"

#define ENABLE_ATOMIC 1

static inline void Uint8Inc(volatile uint8_t *val)
{
    val[0]++;
}

static inline void Uint8Dec(volatile uint8_t *val)
{
    val[0]--;
}

static inline void Uint16Inc(volatile uint16_t *val)
{
    val[0]++;
}

static inline void Uint16Dec(volatile uint16_t *val)
{
    val[0]--;
}

static inline void Uint32Inc(volatile uint32_t *val)
{
    val[0]++;
}

static inline void Uint32Dec(volatile uint32_t *val)
{
    val[0]--;
}

#define TEST_COUNT_TIMES (20000)
static volatile uint8_t atomicUint8Counter = 0;
static volatile uint16_t atomicUint16Counter = 0;
static volatile uint32_t atomicUint32Counter = 0;
static volatile bool atomicTestDone = false;

void AtomicTestMain(void)
{
    while (1)
    {
        static uint32_t incCount = 0;

        if (incCount < TEST_COUNT_TIMES + 100)
        {
            incCount++;
#if ENABLE_ATOMIC
            AtomicOnByte(&atomicUint8Counter, Uint8Inc);
            AtomicOnHaflWord(&atomicUint16Counter, Uint16Inc);
            AtomicOnWord(&atomicUint32Counter, Uint32Inc);
            for (int i = 0; i < 100; i++)
            {
                AtomicOnByte(&atomicUint8Counter, Uint8Inc);
                AtomicOnHaflWord(&atomicUint16Counter, Uint16Inc);
                AtomicOnWord(&atomicUint32Counter, Uint32Inc);

                AtomicOnByte(&atomicUint8Counter, Uint8Dec);
                AtomicOnHaflWord(&atomicUint16Counter, Uint16Dec);
                AtomicOnWord(&atomicUint32Counter, Uint32Dec);
            }

#else
            atomicUint8Counter++;
            for (int i = 0; i < 100; i++)
            {
                atomicUint8Counter++;
                atomicUint8Counter--;
            }

            atomicUint16Counter++;
            for (int i = 0; i < 100; i++)
            {
                atomicUint16Counter++;
                atomicUint16Counter--;
            }

            atomicUint32Counter++;
            for (int i = 0; i < 100; i++)
            {
                atomicUint32Counter++;
                atomicUint32Counter--;
            }
#endif
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        if (atomicTestDone)
        {
            __BKPT(1);
        }
    }
}

void AtomicTestIsr(void)
{
    static uint32_t incCount = 0;

    if (incCount < TEST_COUNT_TIMES)
    {
        incCount++;
#if ENABLE_ATOMIC
        AtomicOnByte(&atomicUint8Counter, Uint8Dec);
        AtomicOnHaflWord(&atomicUint16Counter, Uint16Dec);
        AtomicOnWord(&atomicUint32Counter, Uint32Dec);
#else
        atomicUint8Counter--;
        atomicUint16Counter--;
        atomicUint32Counter--;
#endif
    }
    else
    {
        atomicTestDone = true;
    }
}

static volatile uint8_t ldrexTestValue = 10;
static volatile bool getInIsr = false;
void LdrexTestMain(void)
{
    uint8_t val;
    val = __LDREXB(&ldrexTestValue);

    getInIsr = false;
    while (!getInIsr)
        ;

    uint8_t res = (__STREXB(val, &ldrexTestValue));

    static uint32_t failCount = 0, successCount = 0;
    if (res)
        failCount++;
    else
        successCount++;
}

void LdrexTestIsr(void)
{
    getInIsr = true;
}
