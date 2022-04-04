#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <string>
#include <list>

// TODO: Move static const variables to proper header file
static const float MIN_CUPS = 0.001f;
static const float MAX_CUPS = 8.0f;

static const uint32_t DETECTION_TIME = 3000;

static const uint8_t TX_PIN = D8; // 0/O, CS, Boot fails if HIGH
static const uint8_t RX_PIN = D7; // I/O, MOSI
static const uint8_t PULSE_PIN = D6; // I/O, MISO
static const uint8_t DIR_PIN = D5; // I/O, SCLK
static const uint8_t D4_PIN = D4; // 3v3/O, HIGH at boot, connected to on-board LED, boot fails if LOW
static const uint8_t D3_PIN = D3; // 3v3/O, Connected to flash btn, boot fails if LOW
static const uint8_t BTN_PIN = D2; // I/O, Often used as SDA
static const uint8_t R_LED_PIN = D1; // I/O, Often used as SCL
static const uint8_t G_LED_PIN = D0; // HIGH at boot

static const char RECORD_SEPARATOR = ','; // TODO: Replace with 0x1E
static const char UNIT_SEPARATOR = '&'; // TODO: Replace with 0x1F
static const char PROPERTY_SEPARATOR = ':';
static const char NULL_CHAR = (char) 0x00;
static const char BTE_EOT = (char) 0xFF; // Bluetooth end of transmission

extern SoftwareSerial bt_serial;

extern std::string g_ssid;
extern std::string g_password;

//WiFiClient wifi_client;

#endif
