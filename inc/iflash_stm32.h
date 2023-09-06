#include "cl_common.h"

CL_Result_t IFlashStm32_ErasePages(uint32_t addr, uint32_t pages);
CL_Result_t IFlashStm32_Write(uint32_t addr, const uint8_t *buff, uint32_t length);


