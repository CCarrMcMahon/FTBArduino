#include "packet.h"

std::string Data::getKey() {
    return _key;
}

void Data::setKey(std::string key) {
    _key = key;
}

std::string Data::getValue() {
    return _value;
}

void Data::setValue(std::string value) {
    _value = value;
}

Data::Data() {
    setKey("");
    setValue("");
}

Data::Data(std::string key, std::string value) {
    setKey(key);
    setValue(value);
}

PacketID Packet::getID() {
    return _id;
}

void Packet::setID(PacketID id) {
    _id = id;
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
    Packet::setID(PacketID::NONE);
    Packet::clearData();
}

Packet::Packet(PacketID id, Data data) {
    Packet::setID(id);
    Packet::addData(data);
}
