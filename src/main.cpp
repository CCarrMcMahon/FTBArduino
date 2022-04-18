#include "main.h"

/**
 * Note, Disconnect D3 when programming
 */

void setup() {
    pinMode(TX_PIN, OUTPUT);
    pinMode(RX_PIN, INPUT);
    pinMode(PULSE_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(D4_PIN, OUTPUT);
    pinMode(D3_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT);
    pinMode(R_LED_PIN, OUTPUT);
    pinMode(G_LED_PIN, OUTPUT);

    digitalWrite(TX_PIN, LOW);
    digitalWrite(PULSE_PIN, LOW);
    digitalWrite(DIR_PIN, LOW);
    digitalWrite(D4_PIN, LOW);
    digitalWrite(D3_PIN, LOW);
    digitalWrite(R_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, LOW);

    Serial.begin(9600);

    while (!Serial) {
        
    }

    bt_serial.begin(9600);

    while (!bt_serial) {
        
    }

    EEPROM.begin(EEPROM_SIZE);

    if (read_from_eeprom()) {
        Serial.printf("Stored variables found\n");
        Serial.printf("SSID: %s\nPassword: %s\nMAC: %s\n", g_ssid.c_str(), g_password.c_str(), g_mac.c_str());

        wifi_check();
    } else {
        Serial.printf("No stored variables found\n");
        digitalWrite(R_LED_PIN, HIGH);
        digitalWrite(G_LED_PIN, HIGH);
    }
}

void loop() {
    check_handle_packet();
    check_send_packet();
}
