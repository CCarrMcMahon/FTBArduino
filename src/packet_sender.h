#ifndef __PACKET_SENDER_H
#define __PACKET_SENDER_H

#include "packet_handler.h"

void check_send_packet();
void check_if_animal();
void send_packet(Packet packet);
bool try_connect_website();
bool post_to_website(std::string php_file, std::string postData);

#endif
