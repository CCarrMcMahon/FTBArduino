#include "packet_sender.h"

uint32_t start_time = millis();
uint32_t end_time = start_time;
bool timer_running = false;
bool animal_packet_sent = false;

std::string response_data = "";

void check_send_packet() {
    check_if_animal();
}

void check_if_animal() {
    if (!timer_running) {
        if (digitalRead(BTN_PIN) == HIGH) {
            timer_running = true;
            animal_packet_sent = false;
            start_time = millis();
        }
    } else {
        // If the timer has been running long enough, send a packet if haven't done so
        if (!animal_packet_sent && millis() - start_time >= DETECTION_TIME) {
            animal_packet_sent = true;

            // Display warning if no MAC address is set. They need to add or re-add the device.
            if (g_mac.empty()) {
                digitalWrite(R_LED_PIN, HIGH);
                digitalWrite(G_LED_PIN, HIGH);
                Serial.printf("ERROR: MAC address empty\n");
                Serial.printf("SSID: %s\nPassword: %s\nMAC: %s\n", g_ssid.c_str(), g_password.c_str(), g_mac.c_str());
                return;
            } else {
                digitalWrite(R_LED_PIN, LOW);
                digitalWrite(G_LED_PIN, LOW);
            }

            Packet packet = Packet(PacketID::ANIMAL, Data("mac", g_mac.c_str()));
            send_packet(packet);
        }

        if (digitalRead(BTN_PIN) == LOW) {
            timer_running = false;
        }
    }
}

void send_packet(Packet packet) {
    PacketID id = packet.getID();
    std::list<Data> data = packet.getData();

    std::string postData = "";

    switch (id) {
        case PacketID::ANIMAL:
            postData.append("id=animal");
            break;
        
        default:
            return;
    }

    for (std::list<Data>::iterator it = data.begin(); it != data.end(); ++it) {
        postData.append(1, PROPERTY_SEPARATOR);
        postData.append(it->getKey());
        postData.append(1, PAIR_SEPARATOR);
        postData.append(it->getValue());
    }

    if (post_to_website("PacketHandler.php", postData)) {
        Packet packet = string_to_packet(response_data);
        handle_packet(packet);
    }
}

bool try_connect_website() {
    // If not currently connected to the website, try to connect
    if (!my_client.connected()) {
        int connection_time = 0;

        Serial.printf("\nTrying to connect to the the host: %s...\n", ROOT.c_str());
        Serial.printf("Time Required: ");

        // Wait for connection to website
        while (!my_client.connect(ROOT.c_str(), 80)) {
            // If unable to connect for more than 10 seconds, return
            if (connection_time >= 10) {
                Serial.printf("\nError: Host connection timed out: %s.\n", ROOT.c_str());
                return false;
            }

            Serial.printf("%ds, ", connection_time);
            connection_time++;
            delay(1000);
        }
    }

    Serial.printf("\n\nConnected to the host: %s.\n", ROOT.c_str());
    
    return true;
}

bool post_to_website(std::string php_file, std::string postData) {
    if (!my_client.connected()) {
        if (!try_connect_website()) {
            if (!wifi_check()) {
                return false;
            }

            if (!try_connect_website()) {
                return false;
            }

            if (!my_client.connected()) {
                return false;
            }
        }
    }

    my_client.printf("POST %s HTTP/1.1\n", (DIRECTORY + php_file).c_str());
    my_client.printf("Host: %s\n", ROOT.c_str());
    my_client.printf("User-Agent: Arduino/1.0\n");
    my_client.printf("Content-Length: %i\n", postData.length());
    my_client.printf("Content-Type: application/x-www-form-urlencoded\n");
    my_client.printf("\n");
    my_client.printf("%s", postData.c_str());

    if (!my_client.available()) {
        int connection_time = 0;

        Serial.printf("Waiting for response from the website: %s...\n", (SITE + php_file).c_str());
        Serial.printf("Time Required: ");

        while (!my_client.available()) {
            // If no response for more than 10 seconds, return
            if (connection_time >= 10) {
                Serial.printf("\nError: Website reponse timed out: %s.\n", (SITE + php_file).c_str());
                return false;
            }

            Serial.printf("%ds, ", connection_time);
            connection_time++;
            delay(1000);
        }
    }

    std::string response = my_client.readString().c_str();
    int response_data_pos = response.find("id=");
    response_data = response.substr(response_data_pos);

    Serial.printf("\n\nData Recieved: %s\n", response_data.c_str());

    return true;
}
