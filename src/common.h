#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <string>

const uint8_t PACKET_ID_LEN = 8;

extern std::string g_ssid;
extern std::string g_pass;

//WiFiClient wifi_client;

#endif