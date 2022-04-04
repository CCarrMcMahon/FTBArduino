#include "packet_sender.h"

uint32_t start_time = millis();
uint32_t end_time = start_time;
bool timer_running = false;
bool animal_packet_sent = false;

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

            Packet packet = Packet(PacketType::ANIMAL, Data("status", "true"));
            send_packet(packet);
        }

        if (digitalRead(BTN_PIN) == LOW) {
            timer_running = false;
        }
    }
}

void send_packet(Packet packet) {
    PacketType type = packet.getType();
    std::list<Data> data = packet.getData();

    std::string message = "";

    switch (type) {
        case PacketType::ANIMAL:
            message.append("__animal");
            break;
        
        default:
            return;
    }

    message.append(1, RECORD_SEPARATOR);

    for (std::list<Data>::iterator it = data.begin(); it != data.end(); ++it) {
        message.append(it->getName());
        message.append(1, PROPERTY_SEPARATOR);
        message.append(it->getValue());
        message.append(1, UNIT_SEPARATOR);
    }

    bt_serial.print(message.c_str());
}
