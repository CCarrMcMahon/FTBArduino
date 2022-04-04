#include "packet.h"

std::string Data::getName() {
    return _name;
}

void Data::setName(std::string name) {
    _name = name;
}

std::string Data::getValue() {
    return _value;
}

void Data::setValue(std::string value) {
    _value = value;
}

Data::Data() {
    setName("");
    setValue("");
}

Data::Data(std::string name, std::string value) {
    setName(name);
    setValue(value);
}

PacketType Packet::getType() {
    return _type;
}

void Packet::setType(PacketType type) {
    _type = type;
}

std::list<Data> Packet::getData() {
    return _data;
}

void Packet::setData(std::list<Data> data) {
    _data = data;
}

void Packet::addData(Data data) {
    _data.push_back(data);
}

void Packet::clearData() {
    _data.clear();
}

Packet::Packet() {
    Packet::setType(PacketType::NONE);
    Packet::clearData();
}

Packet::Packet(PacketType type, Data data) {
    Packet::setType(type);
    Packet::addData(data);
}
