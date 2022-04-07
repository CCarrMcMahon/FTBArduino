#include "packet_handler.h"

Packet string_to_packet(std::string received_string) {
    Packet packet = Packet();
    std::size_t property_separator_pos = -1;

    Serial.printf("\nReceived String: %s\n", received_string.c_str());

    do {
        received_string = received_string.substr(property_separator_pos + 1);
        property_separator_pos = received_string.find_first_of(PROPERTY_SEPARATOR);

        if (property_separator_pos == std::string::npos) {
            property_separator_pos = received_string.length();
        }

        std::string pair_string = received_string.substr(0, property_separator_pos);
        std::size_t pair_separator_pos = pair_string.find_first_of(PAIR_SEPARATOR);

        if (pair_separator_pos != std::string::npos) {
            std::string key = pair_string.substr(0, pair_separator_pos);
            std::string value = pair_string.substr(pair_separator_pos + 1);

            // Make sure the key property isn't empty
            if (!key.empty()) {
                // If the property is the class ID, it defines the PacketID
                if (!key.compare("id")) {
                    if (!value.compare("connect")) {
                        packet.setID(PacketID::CONNECT);
                    } else if (!value.compare("givefood")) {
                        packet.setID(PacketID::GIVEFOOD);
                    }
                } else {
                    // Otherwise, it defines the data
                    packet.addData(Data(key, value));
                }
            }
        }
    } while (property_separator_pos != received_string.length());
    
    return packet;
}

void check_handle_packet() {
    // If data has been recieved by the Bluetooth device
    if (bt_serial.available()) {
        std::string received_string = bt_serial.readString().c_str();
        Packet packet = string_to_packet(received_string);

        if (handle_packet(packet)) {
            bt_serial.printf("SUCCESS");
        } else {
            bt_serial.printf("FAIL");
        }
    }
}

bool handle_packet(Packet packet) {
    PacketID id = packet.getID();
    std::list<Data> data = packet.getData();

    Serial.printf("\nID: %d\n", id);
    Serial.printf("Data Size: %d\n", data.size());
    
    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        Serial.printf("Key: %s\tValue: %s\n", iter->getKey().c_str(), iter->getValue().c_str());
    }

    bool result = false;

    switch (id) {
        case PacketID::CONNECT:
            result = try_connect(data);
            break;

        case PacketID::GIVEFOOD:
            result = try_give_food(data);
            break;

        default:
            Serial.printf("No matching packet found\n");
            break;
    }

    return result;
}

bool try_connect(std::list<Data> data) {
    std::string ssid = "";
    std::string password = "";
    std::string mac = "";
    bool found_ssid = false;
    bool found_password = false;
    bool found_mac = false;

    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        if (!iter->getKey().compare("ssid")) {
            ssid = iter->getValue();
            found_ssid = true;
            continue;
        }

        if (!iter->getKey().compare("password")) {
            password = iter->getValue();
            found_password = true;
            continue;
        }

        if (!iter->getKey().compare("mac")) {
            mac = iter->getValue();
            found_mac = true;
            continue;
        }
    }

    if (!found_ssid || !found_password || !found_mac) {
        return false;
    }

    return try_connect_wifi(ssid, password, mac);
}

bool try_give_food(std::list<Data> data) {
    std::string cups_str = "";
    bool found_cups = false;

    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        if (!iter->getKey().compare("cups")) {
            cups_str = iter->getValue();
            found_cups = true;
            break;
        }
    }

    if (!found_cups) {
        return false;
    }

    const char *cups_c_str = cups_str.c_str();

    // Convert string to float
    float cups = strtof(cups_c_str, NULL);

    // Dog not allowed to eat
    if (cups == 0.0f) {
        digitalWrite(R_LED_PIN, HIGH);
        delay(2000);
        digitalWrite(R_LED_PIN, LOW);
        return false;
    }

    cups = max(MIN_CUPS, cups);
    cups = min(cups, MAX_CUPS);

    uint32_t pulses = cups * 400 * 15;

    digitalWrite(G_LED_PIN, HIGH);

    while (pulses) {
        digitalWrite(PULSE_PIN, HIGH);
        delay(1);
        digitalWrite(PULSE_PIN, LOW);
        delay(1);
        pulses--;
    }

    digitalWrite(G_LED_PIN, LOW);

    return true;
}
