//
// Created by root on 2/20/23.
//

#ifndef SIMPLE_WEBSITE_SIMULATION_DATABASE_H
#define SIMPLE_WEBSITE_SIMULATION_DATABASE_H
#include "Packet.h"
namespace sim{
    struct DatabaseState{
        double r = std::numeric_limits<double>::infinity();
        std::queue<Packet> queue;
    };

    std::ostream& operator<<(std::ostream& out, const DatabaseState& s) {
        out <<"{r: "<< s.r
            <<" queue_size:"<<s.queue.size()
            <<"}";
        return out;
    }
    class Database : public cadmium::Atomic<DatabaseState>{
    public:
        constexpr static char NAME[] = "DATABASE";
        constexpr static double PROCESS_TIME = 0.01;
        cadmium::BigPort<Packet> resOut;
        cadmium::BigPort<Packet> reqIn;
        Database(const std::string& id): Atomic<DatabaseState>(id, DatabaseState()){
            reqIn = addInBigPort<Packet>("reqIn");
            resOut = addOutBigPort<Packet>("resOut");
        }
        void internalTransition(DatabaseState& s) const override {
            if(!s.queue.empty())
                s.queue.pop();
            s.r = s.queue.empty()?std::numeric_limits<double>::infinity():PROCESS_TIME;
        }

        void externalTransition(DatabaseState &s, double e) const override {
            s.queue.emplace(reqIn.get()->getBag().back());
            if(s.r == std::numeric_limits<double>::infinity()){
                s.r = PROCESS_TIME;
            }
            else{
                s.r = s.r - e;
            }
        }

        void output(const DatabaseState& s) const override {
            auto front = s.queue.front();
            if(front.type == PacketType::REQUEST){
                resOut->addMessage(front.creationTime, PacketType::RESPONSE, NAME, "PAGE");

            }
            else{
                std::stringstream message;
                message<<"invalid packet: " << front;
                throw std::runtime_error(message.str());
            }
        }
        [[nodiscard]] double timeAdvance(const DatabaseState& s) const override {
            return s.r;
        }

    };


}
#endif //SIMPLE_WEBSITE_SIMULATION_DATABASE_H
