#include "packet_handler.h"

bool try_parse_packet(std::string packet) {
    if (packet.length() < PACKET_ID_LEN) {
        return false;
    }

    bool status = false;

    std::string packet_id = packet.substr(0, PACKET_ID_LEN);
    std::string packet_data = packet.substr(PACKET_ID_LEN);

    Serial.printf("Packet ID: %s\tPacket Data: %s\n", packet_id.c_str(), packet_data.c_str());

    status = try_handle_packet(packet_id, packet_data);
    return status;
}

bool try_handle_packet(std::string packet_id, std::string packet_data) {
    bool status = false;

    if (!packet_id.compare("_connect")) {
        status = try_parse__connect(packet_data);
    } else {
        status = false;
    }

    return status;
}

/**
 * @brief Packet used to try to connect to a Wi-Fi network.
 * Contains an SSID and a PASSWORD.
 * Of the form "ssid=...&password=...\0"
 * 
 * @param data 
 */
bool try_parse__connect(std::string data) {
    // ssid=...&password=...\0
    const uint8_t ssid_id_len = 5;
    const uint8_t pass_id_len = 9;

    if (data.length() < 15) {
        return false;
    }

    std::size_t seperator_pos = data.find('&');

    if (seperator_pos == std::string::npos) {
        return false;
    }

    std::string data_ssid = data.substr(0, seperator_pos);
    std::string data_pass = data.substr(seperator_pos + 1);

    std::string ssid = data_ssid.substr(ssid_id_len);
    std::string pass = data_pass.substr(pass_id_len);

    Serial.printf("SSID: %s\tPASS: %s\n", ssid.c_str(), pass.c_str());

    return try_handle__connect(ssid, pass);
}

bool try_handle__connect(std::string ssid, std::string pass) {
    WiFi.begin(ssid.c_str(), pass.c_str());

    Serial.printf("Trying to connect to the Wi-Fi network: %s...\n", ssid.c_str());
    Serial.printf("Time Required: ");

    int connection_time = 0;
  
    // Wait for the Wi-Fi to connect
    while (WiFi.status() != WL_CONNECTED) {
        // If unable to connect for more than 10 seconds, return
        if (connection_time >= 10) {
            Serial.printf("\nError: Unable to connect to the Wi-Fi network: %s.\n", ssid.c_str());
            return false;  
        }

        Serial.printf("%ds, ", connection_time);
        connection_time++;
        delay(1000);
    }

    std::string g_ssid = "";
    std::string g_pass = "";

    g_ssid = ssid;
    g_pass = pass;

    Serial.printf("\n\nConnected to the Wi-Fi network: %s.\n", ssid.c_str());

    IPAddress local_ip = WiFi.localIP();
    Serial.print("Resolved Address: ");
    Serial.println(ip_to_string(local_ip));

    return true;
}

String ip_to_string(IPAddress &ipAddress) {
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}