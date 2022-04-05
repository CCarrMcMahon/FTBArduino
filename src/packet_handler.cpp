#include "packet_handler.h"

void check_handle_packet() {
    // If data has been recieved by the Bluetooth device
    if (bt_serial.available()) {
        Packet packet = receive_packet();

        if (packet.getID() == PacketID::NONE) {
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

Packet receive_packet() {
    Packet packet = Packet();

    std::string received_string = bt_serial.readString().c_str();
    std::size_t property_separator_pos = -1;

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
            break;
    }

    return result;
}

bool try_connect(std::list<Data> data) {
    std::string ssid_str = "";
    std::string password_str = "";
    std::string mac_str = "";
    bool found_ssid = false;
    bool found_password = false;
    bool found_mac = false;

    for (std::list<Data>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        if (!iter->getKey().compare("ssid")) {
            ssid_str = iter->getValue();
            found_ssid = true;
            continue;
        }

        if (!iter->getKey().compare("password")) {
            password_str = iter->getValue();
            found_password = true;
            continue;
        }

        if (!iter->getKey().compare("mac")) {
            mac_str = iter->getValue();
            found_mac = true;
            continue;
        }
    }

    if (!found_ssid || !found_password || !found_mac) {
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

    g_ssid = ssid_str;
    g_password = password_str;
    g_mac = mac_str;

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
