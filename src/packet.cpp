#include "packet.h"

PacketType Packet::getType() {
    return m_type;
}

void Packet::setType(PacketType type) {
    m_type = type;
}

std::list<Data> Packet::getData() {
    return m_data;
}

void Packet::setData(std::list<Data> data) {
    m_data = data;
}

void Packet::addData(Data data) {
    m_data.push_back(data);
}

void Packet::clearData() {
    m_data.clear();
}

Packet::Packet() {
    Packet::setType(PacketType::NONE);
    Packet::clearData();
}

Packet::Packet(PacketType type, Data data) {
    Packet::setType(type);
    Packet::addData(data);
}
