#pragma once

#include "cl_common.h"

// LOG模块支持多线程和中断打印,每次打印用内存池分配一块内存,用临界区保证内存池操作线程安全,单句日志长度有限制
// freertos中,优先级比临界区高的中断ISR里,不要打log
#define LOG_BLOCK_LEN (31)

void LogModule_Init(void); // 需要在其他线程启动前,就执行初始化
void LogModule_Process(void);
void LogPrint(const char *fmt, ...);
