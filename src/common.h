#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <string>
#include <list>
#include <EEPROM.h>

static const std::string ROOT = "eed0-69-80-148-57.ngrok.io";
static const std::string DIRECTORY = "/FTBServer/login_signup/";
static const std::string SITE = "http://" + ROOT + DIRECTORY;

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

static const char PROPERTY_SEPARATOR = '&'; // TODO: Replace with 0x1F
static const char PAIR_SEPARATOR = '=';
static const char NULL_CHAR = (char) 0x00;
static const char BTE_EOT = (char) 0xFF; // Bluetooth end of transmission

extern SoftwareSerial bt_serial;
extern WiFiClient my_client;

static const uint16_t MAX_EEPROM_SIZE = 0x1000; // Max EEPROM size is 4096 uint8_t
static const uint16_t EEPROM_SIZE = 0x0400; // Choosing EEPROM size of 1024 uint8_t
static const uint16_t SSID_ADDR = 0x0100; // Max length of 64, given 256
static const uint16_t PASS_ADDR = 0x0200; // Max length of 64, given 256
static const uint16_t MAC_ADDR = 0x0300; // Max length of 17, given 256
static const uint16_t FREE_EEPROM = 0x0400; // Free EEPROM

extern std::string g_ssid;
extern std::string g_password;
extern std::string g_mac;

void clear_eeprom();
void store_in_eeprom();
bool read_from_eeprom();
bool wifi_check();
bool try_connect_wifi(std::string ssid, std::string password, std::string mac);

#endif
