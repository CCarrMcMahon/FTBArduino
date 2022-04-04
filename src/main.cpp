#include "main.h"

/**
 * Note, Disconnect D3 when programming
 */

void setup() {
    pinMode(TX_PIN, OUTPUT);
    pinMode(RX_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(PULSE_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT);
    pinMode(R_LED_PIN, OUTPUT);
    pinMode(G_LED_PIN, OUTPUT);

    digitalWrite(TX_PIN, LOW);
    digitalWrite(TRIG_PIN, LOW);
    digitalWrite(PULSE_PIN, LOW);
    digitalWrite(DIR_PIN, LOW);
    digitalWrite(R_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, LOW);

    Serial.begin(9600);

    while (!Serial) {
        
    }

    bt_serial.begin(9600);

    while (!bt_serial) {
        
    }
}

void loop() {
    check_handle_packet();
    check_send_packet();
}
