#ifndef __MAIN_H
#define __MAIN_H

#include "packet_sender.h"
#include "packet_handler.h"

SoftwareSerial bt_serial(RX_PIN, TX_PIN);
WiFiClient my_client;

std::string g_ssid = "";
std::string g_password = "";
std::string g_mac = "";

#endif
