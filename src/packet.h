#ifndef __PACKET_H
#define __PACKET_H

#include "common.h"

// Possibly replace with static const variables for easier comparisons
enum PacketID {
    NONE,
    CONNECT,
    GIVEFOOD,
    ANIMAL
};

class Data {
    private:
        std::string _key;
        std::string _value;
    
    public:
        std::string getKey();
        void setKey(std::string key);
        std::string getValue();
        void setValue(std::string value);
        Data();
        Data(std::string key, std::string value);
};

class Packet {
    private:
        PacketID _id;
        std::list<Data> _data;

    public:
        PacketID getID();
        void setID(PacketID id);
        std::list<Data> getData();
        void setData(std::list<Data> data);
        void addData(Data data);
        void clearData();
        Packet();
        Packet(PacketID id, Data data);
};

#endif
