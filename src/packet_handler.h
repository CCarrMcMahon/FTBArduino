#ifndef __PACKET_HANDLER_H
#define __PACKET_HANDLER_H

#include "packet.h"

void check_handle_packet(Stream &bt_serial);
Packet receive_packet(Stream &bt_serial);
PacketType parse_type(std::string received_string);
Data parse_data(std::string received_string);
void handle_packet(Packet packet);
bool try_connect(std::list<Data> data);
String ip_to_string(IPAddress &ipAddress); // TODO: Change to std::string eventually
bool try_give_food(std::list<Data> data);

#endif
