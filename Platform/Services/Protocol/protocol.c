#include "protocol.h"


void protocol_create(Packet *packet,
                     PacketType type,
					 const uint8_t *data,
                     uint16_t length)
{

    packet->header = 0xAA;

    packet->type = type;

    packet->length = length;

    packet->timestamp = 0;


    for(uint16_t i = 0; i < length; i++)
    {
        packet->payload[i] = data[i];
    }


    /* Checksum generation reserved for future implementation. */
    packet->checksum = 0;

}
