#pragma once

#include "cl_common.h"
#include "mmlib_config.h"

//data max length
#define MAX_MSG_DATA_LEN 256
typedef struct
{
    uint8_t cmd;
    uint8_t subCmd;
    uint8_t length;
    uint8_t data[256];
} SgpPacket_t; //receive massage

//timeout of one frame, ms
#define SGP_FRAME_TIMEOUT (500)

typedef CL_Result_t (*SGP_SendFunc)(const uint8_t *buff, uint16_t count);

//init, need send function
void SgpProtocol_Init(void);

void SgpProtocol_AddChannel(SpgChannelHandle_t handle, SGP_SendFunc s);
void SgpProtocol_RmChannel(SpgChannelHandle_t handle);

//send sgp massage
CL_Result_t SgpProtocol_SendMsg(SpgChannelHandle_t handle, uint8_t cmd, uint8_t subCmd, const uint8_t *data, uint8_t length);


//raise event with session = 0
void SgpProtocol_RecvByte(SpgChannelHandle_t handle, uint8_t byte);
void SgpProtocol_RecvData(SpgChannelHandle_t handle, const uint8_t *buff, uint16_t count);
