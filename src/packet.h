#ifndef __PACKET_H
#define __PACKET_H

#include "common.h"

// Possibly replace with static const variables for easier comparisons
enum PacketType {
    NONE,
    CONNECT,
    GIVEFOOD,
    ANIMAL
};

class Data {
    private:
        std::string _name;
        std::string _value;
    
    public:
        std::string getName();
        void setName(std::string name);
        std::string getValue();
        void setValue(std::string value);
        Data();
        Data(std::string name, std::string value);
};

class Packet {
    private:
        PacketType _type;
        std::list<Data> _data;

    public:
        PacketType getType();
        void setType(PacketType type);
        std::list<Data> getData();
        void setData(std::list<Data> data);
        void addData(Data data);
        void clearData();
        Packet();
        Packet(PacketType type, Data data);
};

#endif
