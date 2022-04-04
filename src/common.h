#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <string>
#include <list>

// TODO: Move static const variables to proper header file
static const float MIN_CUPS = 0.001f;
static const float MAX_CUPS = 16.0f;

static const uint32_t DETECTION_TIME = 5000;

static const uint8_t TX_PIN = D8; // CS, Boot fails if HIGH
static const uint8_t RX_PIN = D7; // MOSI
static const uint8_t TRIG_PIN = D6; // MISO
static const uint8_t ECHO_PIN = D5; // SCLK
static const uint8_t PULSE_PIN = D4; // HIGH at boot, connected to on-board LED, boot fails if LOW
static const uint8_t DIR_PIN = D3; // Connected to flash btn, boot fails if LOW
static const uint8_t BTN_PIN = D2; // Often used as SDA
static const uint8_t R_LED_PIN = D1; // Often used as SCL
static const uint8_t G_LED_PIN = D0; // HIGH at boot

static const uint8_t SONAR_SAMPLES = 10;
static const uint8_t MAX_FOOD_DIS_CM = 20;

static const char RECORD_SEPARATOR = ','; // TODO: Replace with 0x1E
static const char UNIT_SEPARATOR = '&'; // TODO: Replace with 0x1F
static const char PROPERTY_SEPARATOR = ':';
static const char NULL_CHAR = (char) 0x00;
static const char BTE_EOT = (char) 0xFF; // Bluetooth end of transmission

extern SoftwareSerial bt_serial;
extern NewPing sonar;

extern std::string g_ssid;
extern std::string g_password;

extern bool gave_food;

//WiFiClient wifi_client;

#endif
