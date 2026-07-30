#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>


typedef enum
{
    PACKET_SENSOR = 0x01,
    PACKET_STATUS = 0x02,
    PACKET_COMMAND = 0x03

} PacketType;


typedef struct
{
    uint8_t header;

    uint8_t type;

    uint16_t length;

    uint32_t timestamp;

    uint8_t payload[32];

    uint8_t checksum;

} Packet;


void protocol_create(Packet *packet,
                     PacketType type,
					 const uint8_t *data,
                     uint16_t length);


#endif
