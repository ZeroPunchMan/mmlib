#include "cl_common.h"

uint16_t Modbus_CRC16(const uint8_t *buf, unsigned int len);
uint16_t Modbus_CRC16Ex(const uint8_t *buf, unsigned int len, uint16_t initCrc);
uint32_t Ethernet_CRC32(const uint8_t *data, int count);

