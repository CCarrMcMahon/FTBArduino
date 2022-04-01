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
    // check_send_packet();
    check_handle_packet(bt_serial);

    // if (Serial.available()) {
    //     bt_serial.println(Serial.readString());
    // }

    // if (bt_serial.available()) {
    //     received_data = build_packet();
    //     Serial.printf("\n\nReceived Data: %s\n", received_data.c_str());
        
    //     if (try_parse_packet(received_data)) {
    //         bt_serial.print("Success");
    //     } else {
    //         bt_serial.print("Fail");
    //     }
    // }
}
