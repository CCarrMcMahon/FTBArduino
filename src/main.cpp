#include "main.h"

void setup() {
    Serial.begin(9600);

    while (!Serial) {
        
    }

    bt_serial.begin(9600);

    while (!bt_serial) {
        
    }
}

void loop() {
    if (Serial.available()) {
        bt_serial.println(Serial.readString());
    }

    if (bt_serial.available()) {
        received_data = build_packet();
        Serial.printf("\n\nReceived Data: %s\n", received_data.c_str());
        
        if (try_parse_packet(received_data)) {
            bt_serial.print("Success");
        } else {
            bt_serial.print("Fail");
        }
    }
}

std::string build_packet() {
    char received_char = (char) 0xFF;
    std::string received_data = "";

    do {
        received_char = bt_serial.read();

        if (received_char == (char) 0xFF) {
            delay(2000);
            received_char = bt_serial.read();

            if (received_char == (char) 0xFF) {
                received_char = (char) 0x00;
            }
        }

        received_data.append(1, received_char);
    } while (received_char != (char) 0x00);

    return received_data;
}