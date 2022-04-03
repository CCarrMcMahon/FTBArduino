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

static const uint8_t TX_PIN = D8;
static const uint8_t RX_PIN = D7;
static const uint8_t R_LED_PIN = D6;
static const uint8_t G_LED_PIN = D5;
static const uint8_t PULSE_PIN = D4;
static const uint8_t DIR_PIN = D3;
static const uint8_t TRIG_PIN = D2;
static const uint8_t ECHO_PIN = D1;
static const uint8_t BTN_PIN = D0;

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
