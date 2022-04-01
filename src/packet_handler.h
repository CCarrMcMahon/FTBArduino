#ifndef __PACKET_HANDLER_H
#define __PACKET_HANDLER_H

#include "packet.h"

void check_handle_packet(Stream &bt_serial);
Packet receive_packet(Stream &bt_serial);
PacketType parse_type(std::string received_string);
Data parse_data(std::string received_string);

// bool try_parse_packet(std::string packet);
// bool try_handle_packet(std::string packet_id, std::string packet_data);
// bool try_parse__connect(std::string data);
// bool try_handle__connect(std::string ssid, std::string pass);
// String ip_to_string(IPAddress &ipAddress);

#endif