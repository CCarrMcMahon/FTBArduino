#include "packet_sender.h"

// TODO:
// Add ultrasonic sensors or button
//     For animal sensor, check if blocked for a certain amount of time
//     Once blocked, send an animal packet to the webserver
//     When recieved, compare curent time and when the animal last ate to schedule
//     If not allowed, send give_food packet back with 0 cups
//         If 0 cups, turn on red light for certain amount of time
//         Possibly add global delay for how long light is on and when to check food again
//     If allowed, send give_food packet back with proper cups
//         Turn on green light until food is done

void check_send_packet() {
    check_if_animal();
    check_if_food_low();
}

void check_if_animal() {
    return;
}

void check_if_food_low() {
    if (!gave_food) {
        return;
    }

    gave_food = false;

    uint32_t avg_time = sonar.ping_median(SONAR_SAMPLES);
    uint32_t avg_dis_cm = avg_time / ((unsigned long) US_ROUNDTRIP_CM);
    std::string avg_dis_str = std::to_string(avg_dis_cm);

    if (avg_dis_cm >= MAX_FOOD_DIS_CM) {
        Packet packet = Packet(PacketType::FOOD_LOW, Data("status", "true"));
        packet.addData(Data("distance", avg_dis_str)); // TODO: Remove, used for testing
        send_packet(packet);
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

        case PacketType::FOOD_LOW:
            message.append("food_low");
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
