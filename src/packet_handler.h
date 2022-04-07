#ifndef __PACKET_HANDLER_H
#define __PACKET_HANDLER_H

#include "packet.h"

Packet string_to_packet(std::string received_string);
void check_handle_packet();
bool handle_packet(Packet packet);
bool try_connect(std::list<Data> data);
bool try_give_food(std::list<Data> data);

#endif
