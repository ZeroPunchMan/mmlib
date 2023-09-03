#pragma once

#include "stdint.h"
#include "cmsis_compiler.h"

typedef void (*OperationOnByte)(volatile uint8_t *val);

static inline void AtomicOnByte(volatile uint8_t *n, OperationOnByte op)
{
    uint8_t val;
    do
    {
        val = __LDREXB(n);
        op(&val);
    } while ((__STREXB(val, n)) != 0U);
}

typedef void (*OperationOnHaflWord)(volatile uint16_t *val);
static inline void AtomicOnHaflWord(volatile uint16_t *n, OperationOnHaflWord op)
{
    uint16_t val;
    do
    {
        val = __LDREXH(n);
        op(&val);
    } while ((__STREXH(val, n)) != 0U);
}

typedef void (*OperationOnWord)(volatile uint32_t *val);
static inline void AtomicOnWord(volatile uint32_t *n, OperationOnWord op)
{
    uint32_t val;
    do
    {
        val = __LDREXW(n);
        op(&val);
    } while ((__STREXW(val, n)) != 0U);
}
