#include "packet_handler.h"

void check_handle_packet() {
    // If data has been recieved by the Bluetooth device
    if (bt_serial.available()) {
        Packet packet = receive_packet();

        if (packet.getType() == PacketType::NONE) {
            Serial.printf("No matching packet found\n");
            return;
        }

        if (handle_packet(packet)) {
            bt_serial.printf("SUCCESS");
        } else {
            bt_serial.printf("FAIL");
        }
    }
}

/**
 * TODO: Possibly remove the record separator value and consolidate the unit separator.
 * That way, I can think of the packet as a list of items.
 * Then, I can count each time a unit separator is reached, and consider the first one the "id".
 * I would also need to add the "id" name to the first data packet to keep things consistant.
 */
Packet receive_packet() {
    std::string received_string = "";
    char received_char = BTE_EOT;
    Packet packet = Packet();
    PacketType packet_type;
    Data data;
    bool new_timeout = true;
    bool needs_last_data = true;

    do {
        received_char = bt_serial.read();

        if (!needs_last_data) {
            received_char = UNIT_SEPARATOR;
        }

        switch (received_char) {
            // If received character is a record separator
            case RECORD_SEPARATOR:
                packet_type = parse_type(received_string);

                packet.setType(packet_type);
                received_string = "";

                if (!new_timeout) {
                    new_timeout = true;
                }

                break;

            // If received character is a unit separator
            case UNIT_SEPARATOR:
                data = parse_data(received_string);

                // If the data name is not empty, add it to the packet
                if (!data.getName().empty()) {
                    packet.addData(data);
                }
                
                received_string = "";

                if (!new_timeout) {
                    new_timeout = true;
                }

                if (!needs_last_data) {
                    received_char = (char) 0x00;
                }
                
                break;

            // If received character is Bluetooth end of transmission
            case BTE_EOT:
                if (new_timeout) {
                    new_timeout = false;
                    delay(2000);
                    break;
                }

                if (needs_last_data) {
                    needs_last_data = false;
                    break;
                }
                
                received_char = NULL_CHAR;
                break;

            default:
                received_string.append(1, received_char);

                if (!new_timeout) {
                    new_timeout = true;
                }
               
                break;
        }
    } while (received_char != NULL_CHAR);

    return packet;
}

PacketType parse_type(std::string received_string) {
    if (!received_string.compare("_connect")) {
        return PacketType::CONNECT;
    }
    
    if (!received_string.compare("givefood")) {
        return PacketType::GIVEFOOD;
    }

    return PacketType::NONE;
}

Data parse_data(std::string received_string) {
    Data data = Data();

    std::size_t seperator_pos = received_string.find_first_of(PROPERTY_SEPARATOR);

    if (seperator_pos == std::string::npos) {
        return data;
    }

    std::string name = received_string.substr(0, seperator_pos);
    std::string value = received_string.substr(seperator_pos + 1);

    data.setName(name);
    data.setValue(value);

    return data;
}

bool handle_packet(Packet packet) {
    PacketType type = packet.getType();
    std::list<Data> data = packet.getData();

    Serial.printf("\nType: %d\n", type);
    Serial.printf("Data Size: %d\n", data.size());
    
    for (std::list<Data>::iterator it = data.begin(); it != data.end(); ++it) {
        Serial.printf("Name: %s\tValue: %s\n", it->getName().c_str(), it->getValue().c_str());
    }

    bool result = false;

    switch (type) {
        case PacketType::CONNECT:
            result = try_connect(data);
            break;

        case PacketType::GIVEFOOD:
            result = try_give_food(data);
            break;

        default:
            break;
    }

    return result;
}

bool try_connect(std::list<Data> data) {
    std::string ssid_str = "";
    std::string password_str = "";
    bool found_ssid = false;
    bool found_password = false;

    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        if (!iter->getName().compare("ssid")) {
            ssid_str = iter->getValue();
            found_ssid = true;
            continue;
        }

        if (!iter->getName().compare("password")) {
            password_str = iter->getValue();
            found_password = true;
            continue;
        }
    }

    if (!found_ssid) {
        return false;
    }

    if (!found_password) {
        return false;
    }

    const char *ssid = ssid_str.c_str();
    const char *password = password_str.c_str();

    WiFi.begin(ssid, password);

    Serial.printf("\nTrying to connect to the Wi-Fi network: %s...\n", ssid);
    Serial.printf("With password: %s\n", password);
    Serial.printf("Time Required: ");

    int connection_time = 0;
  
    // Wait for the Wi-Fi to connect
    while (WiFi.status() != WL_CONNECTED) {
        // If unable to connect for more than 10 seconds, return
        if (connection_time >= 10) {
            Serial.printf("\nError: Unable to connect to the Wi-Fi network: %s.\n", ssid);
            return false;  
        }

        Serial.printf("%ds, ", connection_time);
        connection_time++;
        delay(1000);
    }

    g_ssid = ssid;
    g_password = password;

    Serial.printf("\n\nConnected to the Wi-Fi network: %s.\n", ssid);

    IPAddress local_ip = WiFi.localIP();
    Serial.print("Resolved Address: ");
    Serial.println(ip_to_string(local_ip));

    WiFi.disconnect(); // TODO: Remove this (Maybe, I have to reconnect to internet later anyway)

    return true;
}

String ip_to_string(IPAddress &ipAddress) {
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}

bool try_give_food(std::list<Data> data) {
    std::string cups_str = "";
    bool found_cups = false;

    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        if (!iter->getName().compare("cups")) {
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
        // TODO: Handle turning on and off red light along with delay for re-running this function
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
    
    gave_food = true;

    return true;
}
