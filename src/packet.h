#ifndef __PACKET_H
#define __PACKET_H

#include "common.h"

enum PacketType {
    CONNECT,
    GIVEFOOD,
    ANIMAL,
    LOWFOOD,
    NONE
};

struct Data {
    std::string name;
    std::string value;
};

class Packet {
    private:
        PacketType m_type;
        std::list<Data> m_data;

    public:
        Packet();
        Packet(PacketType type, Data data);
        PacketType getType();
        void setType(PacketType type);
        std::list<Data> getData();
        void setData(std::list<Data> data);
        void addData(Data data);
        void clearData();
};

#endif