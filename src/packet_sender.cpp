#include "packet_sender.h"

uint32_t start_time = millis();
uint32_t end_time = start_time;
bool timer_running = false;
bool animal_packet_sent = false;

void check_send_packet() {
    check_if_animal();
}

void check_if_animal() {
    if (g_mac.empty()) {
        return;
    }

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

    std::string message = "";

    switch (id) {
        case PacketID::ANIMAL:
            message.append("id:animal");
            break;
        
        default:
            return;
    }

    for (std::list<Data>::iterator it = data.begin(); it != data.end(); ++it) {
        message.append(1, PROPERTY_SEPARATOR);
        message.append(it->getKey());
        message.append(1, PAIR_SEPARATOR);
        message.append(it->getValue());
    }

    bt_serial.print(message.c_str());
}
