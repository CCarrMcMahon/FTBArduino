#include "packet_handler.h"

void check_handle_packet(Stream &bt_serial) {
    // If data has been recieved by the Bluetooth device
    if (bt_serial.available()) {
        Packet packet = receive_packet(bt_serial);

        if (packet.getType() == PacketType::NONE) {
            return;
        }

        Serial.printf("Type: %d\n", packet.getType());
        
        for (std::list<Data>::iterator it = packet.getData().begin(); it != packet.getData().end(); it++) {
            Serial.printf("Name: %s\tValue: %s\n", (*it).name.c_str(), (*it).value.c_str());
        }
    }
}

Packet receive_packet(Stream &bt_serial) {
    std::string received_string = "";
    char received_char = (char) 0xFF;
    Packet packet = Packet();
    PacketType packet_type;
    Data data;

    do {
        received_char = bt_serial.read();

        switch (received_char) {
            // If received character is a record separator
            case ',':
                packet_type = parse_type(received_string);

                if (packet_type == PacketType::NONE) {
                    return packet;
                }

                packet.setType(packet_type);
                received_string = "";
                break;

            // If received character is a unit separator
            case '&':
                data = parse_data(received_string);

                if (!data.name.compare("")) {
                    return packet;
                }

                packet.addData(data);
                received_string = "";
                break;

            // If received character is Bluetooth end of transmission
            case ((char) 0xFF):
                delay(2000);
                received_char = bt_serial.read();

                if (received_char == (char) 0xFF) {
                    received_char = (char) 0x00;
                }

                break;

            default:
                break;
        }

        received_string.append(1, received_char);
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




// bool try_parse_packet(std::string packet) {
//     if (packet.length() < PACKET_ID_LEN) {
//         return false;
//     }

//     bool status = false;

//     std::string packet_id = packet.substr(0, PACKET_ID_LEN);
//     std::string packet_data = packet.substr(PACKET_ID_LEN);

//     Serial.printf("Packet ID: %s\tPacket Data: %s\n", packet_id.c_str(), packet_data.c_str());

//     status = try_handle_packet(packet_id, packet_data);
//     return status;
// }

// bool try_handle_packet(std::string packet_id, std::string packet_data) {
//     bool status = false;

//     if (!packet_id.compare("_connect")) {
//         status = try_parse__connect(packet_data);
//     } else {
//         status = false;
//     }

//     return status;
// }


// bool try_handle__connect(std::string ssid, std::string pass) {
//     WiFi.begin(ssid.c_str(), pass.c_str());

//     Serial.printf("Trying to connect to the Wi-Fi network: %s...\n", ssid.c_str());
//     Serial.printf("Time Required: ");

//     int connection_time = 0;
  
//     // Wait for the Wi-Fi to connect
//     while (WiFi.status() != WL_CONNECTED) {
//         // If unable to connect for more than 10 seconds, return
//         if (connection_time >= 10) {
//             Serial.printf("\nError: Unable to connect to the Wi-Fi network: %s.\n", ssid.c_str());
//             return false;  
//         }

//         Serial.printf("%ds, ", connection_time);
//         connection_time++;
//         delay(1000);
//     }

//     std::string g_ssid = "";
//     std::string g_pass = "";

//     g_ssid = ssid;
//     g_pass = pass;

//     Serial.printf("\n\nConnected to the Wi-Fi network: %s.\n", ssid.c_str());

//     IPAddress local_ip = WiFi.localIP();
//     Serial.print("Resolved Address: ");
//     Serial.println(ip_to_string(local_ip));

//     WiFi.disconnect();

//     return true;
// }

// String ip_to_string(IPAddress &ipAddress) {
//     return String(ipAddress[0]) + String(".") +
//            String(ipAddress[1]) + String(".") +
//            String(ipAddress[2]) + String(".") +
//            String(ipAddress[3]);
// }