#ifndef __MAIN_H
#define __MAIN_H

#include "packet_handler.h"

SoftwareSerial bt_serial(D7, D8);
std::string received_data = "";

std::string build_packet(void);

#endif