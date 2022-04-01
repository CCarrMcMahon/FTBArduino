#include "packet_handler.h"

void check_handle_packet(Stream &bt_serial) {
    // If data has been recieved by the Bluetooth device
    if (bt_serial.available()) {
        Packet packet = receive_packet(bt_serial);

        if (packet.getType() == PacketType::NONE) {
            Serial.printf("No matching packet found\n");
            return;
        }

        handle_packet(packet);
    }
}

Packet receive_packet(Stream &bt_serial) {
    std::string received_string = "";
    char received_char = (char) 0xFF;
    Packet packet = Packet();
    PacketType packet_type;
    Data data;
    bool new_timeout = true;
    bool needs_last_data = true;

    do {
        received_char = bt_serial.read();

        if (!needs_last_data) {
            received_char = '&';
        }

        switch (received_char) {
            // If received character is a record separator
            case ',':
                packet_type = parse_type(received_string);

                packet.setType(packet_type);
                received_string = "";

                if (!new_timeout) {
                    new_timeout = true;
                }

                break;

            // If received character is a unit separator
            case '&':
                data = parse_data(received_string);

                // If the data name is not empty, add it to the packet
                if (!data.name.empty()) {
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
            case ((char) 0xFF):
                if (new_timeout) {
                    new_timeout = false;
                    delay(2000);
                    break;
                }

                if (needs_last_data) {
                    needs_last_data = false;
                    break;
                }
                
                received_char = (char) 0x00;
                break;

            default:
                received_string.append(1, received_char);

                if (!new_timeout) {
                    new_timeout = true;
                }
               
                break;
        }
    } while (received_char != (char) 0x00);

    return packet;
}

PacketType parse_type(std::string received_string) {
    if (!received_string.compare("_connect")) {
        return PacketType::CONNECT;
    }
    
    if (!received_string.compare("givefood")) {
        return PacketType::GIVEFOOD;
    }
    
    if (!received_string.compare("__animal")) {
        return PacketType::ANIMAL;
    }
    
    if (!received_string.compare("low_food")) {
        return PacketType::LOWFOOD;
    }

    return PacketType::NONE;
}

Data parse_data(std::string received_string) {
    Data data;
    data.name = "";
    data.value = "";

    std::size_t seperator_pos = received_string.find_first_of(":");

    if (seperator_pos == std::string::npos) {
        return data;
    }

    data.name = received_string.substr(0, seperator_pos);
    data.value = received_string.substr(seperator_pos + 1);

    return data;
}

void handle_packet(Packet packet) {
    PacketType type = packet.getType();
    std::list<Data> data = packet.getData();

    Serial.printf("\nType: %d\n", type);
    Serial.printf("Data Size: %d\n", data.size());
    
    for (std::list<Data>::iterator it = data.begin(); it != data.end(); ++it) {
        Serial.printf("Name: %s\tValue: %s\n", it->name.c_str(), it->value.c_str());
    }

    switch (type) {
        case PacketType::CONNECT:
            try_connect(data);
            break;

        case PacketType::GIVEFOOD:
            break;

        default:
            break;
    }
}

bool try_connect(std::list<Data> data) {
    const char *ssid;
    const char *password;
    bool found_ssid = false;
    bool found_password = false;

    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        if (!iter->name.compare("ssid")) {
            ssid = iter->value.c_str();
            found_ssid = true;
            continue;
        }

        if (!iter->name.compare("password")) {
            password = iter->value.c_str();
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

    std::string g_ssid = "";
    std::string g_password = "";

    g_ssid = ssid;
    g_password = password;

    Serial.printf("\n\nConnected to the Wi-Fi network: %s.\n", ssid);

    IPAddress local_ip = WiFi.localIP();
    Serial.print("Resolved Address: ");
    Serial.println(ip_to_string(local_ip));

    WiFi.disconnect();

    return true;
}

String ip_to_string(IPAddress &ipAddress) {
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}

bool try_give_food(std::list<Data> data) {
    return true;
}
