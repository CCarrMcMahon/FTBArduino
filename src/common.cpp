#include "common.h"

void clear_eeprom() {
    for (uint16_t i = 0; i < (uint16_t) EEPROM.length(); i++) {
        EEPROM.write(i, 0);
    }
    
    EEPROM.commit();
}

void store_in_eeprom() {
    static const uint16_t g_ssid_len = g_ssid.length();
    static const uint16_t g_password_len = g_password.length();
    static const uint16_t g_mac_len = g_mac.length();

    Serial.printf("Storing values in EEPROM\n");
    Serial.printf("SSID\n");
    for (uint16_t i = 0; i < 256; i++) {
        if (i < g_ssid_len) {
            EEPROM.write(SSID_ADDR + i, g_ssid.at(i));
            Serial.printf("%c", g_ssid.at(i));
        } else {
            EEPROM.write(SSID_ADDR + i, 0x00);
        }
    }

    Serial.printf("\n\nPASSWORD\n");
    for (uint16_t i = 0; i < 256; i++) {
        if (i < g_password_len) {
            EEPROM.write(PASS_ADDR + i, g_password.at(i));
            Serial.printf("%c", g_password.at(i));
        } else {
            EEPROM.write(PASS_ADDR + i, 0x00);
        }
    }

    Serial.printf("\n\nMAC\n");
    for (uint16_t i = 0; i < 256; i++) {
        if (i < g_mac_len) {
            EEPROM.write(MAC_ADDR + i, g_mac.at(i));
            Serial.printf("%c", g_mac.at(i));
        } else {
            EEPROM.write(MAC_ADDR + i, 0x00);
        }
    }

    Serial.printf("\n");

    EEPROM.commit();
}

bool read_from_eeprom() {
    uint8_t character = 0x00;

    for (uint16_t i = SSID_ADDR; i < PASS_ADDR; i++) {
        character = EEPROM.read(i);

        if (character == 0x00) {
            if (i == SSID_ADDR) {
                return false;
            }

            break;
        }

        g_ssid.append(1, (char) character);
    }

    for (uint16_t i = PASS_ADDR; i < MAC_ADDR; i++) {
        character = EEPROM.read(i);

        if (character == 0x00) {
            if (i == PASS_ADDR) {
                return false;
            }

            break;
        }

        g_password.append(1, (char) character);
    }

    for (uint16_t i = MAC_ADDR; i < FREE_EEPROM; i++) {
        character = EEPROM.read(i);

        if (character == 0x00) {
            if (i == MAC_ADDR) {
                return false;
            }

            break;
        }

        g_mac.append(1, (char) character);
    }

    return true;
}

bool wifi_check() {
    if (!try_connect_wifi(g_ssid, g_password, g_mac)) {
        digitalWrite(R_LED_PIN, HIGH);
        digitalWrite(G_LED_PIN, HIGH);
        Serial.printf("ERROR: Device in database but has wrong global parameters or can't find Wi-Fi network\n");
        Serial.printf("SSID: %s\nPassword: %s\nMAC: %s\n", g_ssid.c_str(), g_password.c_str(), g_mac.c_str());
        return false;
    } else {
        digitalWrite(R_LED_PIN, LOW);
        digitalWrite(G_LED_PIN, LOW);
        return true;
    }
}

bool try_connect_wifi(std::string ssid, std::string password, std::string mac) {
    // If not currently connected to a Wi-Fi network, try to connect
    if (!WiFi.isConnected()) {
        WiFi.begin(ssid.c_str(), password.c_str());

        Serial.printf("\nTrying to connect to the Wi-Fi network: %s...\n", ssid.c_str());
        Serial.printf("With password: %s\n", password.c_str());
        Serial.printf("Time Required: ");

        int connection_time = 0;
    
        // Wait for the Wi-Fi to connect
        while (WiFi.status() != WL_CONNECTED) {
            // If unable to connect for more than 10 seconds, return
            if (connection_time >= 10) {
                Serial.printf("\nError: Wi-Fi network connection timed out: %s.\n", ssid.c_str());
                return false;
            }

            Serial.printf("%ds, ", connection_time);
            connection_time++;
            delay(1000);
        }

        g_ssid = ssid;
        g_password = password;
        g_mac = mac;

        store_in_eeprom();

        digitalWrite(R_LED_PIN, LOW);
        digitalWrite(G_LED_PIN, LOW);

        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);
    }

    Serial.printf("\n\nConnected to the Wi-Fi network: %s.\n", g_ssid.c_str());
    Serial.printf("Resolved Address: %s\n", WiFi.localIP().toString().c_str());
    
    return true;
}
