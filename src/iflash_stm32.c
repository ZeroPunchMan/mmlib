#include "iflash_stm32.h"
#include "main.h"

CL_Result_t IFlashStm32_ErasePages(uint32_t addr, uint32_t pages)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = addr;
    EraseInitStruct.NbPages = pages;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
        return CL_ResFailed;

    return CL_ResSuccess;
}

CL_Result_t IFlashStm32_Write(uint32_t addr, const uint8_t *buff, uint32_t length)
{
    uint32_t writeAddr, offset;
    uint32_t data;
    HAL_StatusTypeDef status;

    /* Clear All pending flags */
    // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR | FLASH_FLAG_PROGERR);

    // little endian [0-0][0-1][0-2][0-3][1-0][1-1][1-2][1-3] -> 3210
    offset = 0;
    writeAddr = addr;
    while (offset < length)
    {
        data = 0;
        for (int i = 0; i < 4; i++)
        {
            if (offset < length)
                data |= (uint32_t)buff[offset++] << (i * 8);
            else
                break;
        }
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, writeAddr, data);
        if (status != HAL_OK)
            return CL_ResFailed;

        writeAddr += 4;
    }
    return CL_ResSuccess;
}


