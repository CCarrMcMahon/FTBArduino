#ifndef __MAIN_H
#define __MAIN_H

#include "packet_sender.h"
#include "packet_handler.h"

SoftwareSerial bt_serial(RX_PIN, TX_PIN);
NewPing sonar(TRIG_PIN, ECHO_PIN);

std::string g_ssid = "";
std::string g_password = "";

bool gave_food = false;

#endif
