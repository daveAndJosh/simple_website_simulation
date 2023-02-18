#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_PACKET_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_PACKET_H

#include <ostream>
#include <utility>

namespace sim{
    enum class PacketType{
        REQUEST,
        RESPONSE
    };

    std::ostream& operator<<(std::ostream& out, const PacketType& type){
        std::string s;
        switch(type){
            case PacketType::REQUEST:
                s = "REQUEST";
                break;
            default:
                s = "RESPONSE";
        }
        out<<s;
        return out;
    }

    struct Packet {
        Packet(const Packet &packet) = default;

        double creationTime;
        PacketType type;
        std::string dest;
        std::string source;

        Packet()= default;
        Packet(Packet& packet) = default;

        /**
         *
         * @param ct the creation time
         * @param t the type of packet
         * @param d the destination
         * @param s the source
         */
        Packet(double ct, PacketType t, std::string s,std::string d)
            :creationTime(ct),
            type(t),
            dest(std::move(d)),
            source(std::move(s)){}


    };



    std::ostream& operator<<(std::ostream& out, const Packet& packet){
        out
            <<"{Type:"<<packet.type
            <<", CreationTime:"<<packet.creationTime
            <<", Source:"<<packet.source
            <<", Dest:"<< packet.dest
            <<"}";
        return out;
    }

}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_PACKET_H
