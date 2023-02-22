//
// Created by root on 2/20/23.
//

#ifndef SIMPLE_WEBSITE_SIMULATION_BUCKET_H
#define SIMPLE_WEBSITE_SIMULATION_BUCKET_H
#include <cadmium/core/modeling/atomic.hpp>
#include <queue>
#include <limits>
#include <iostream>
#include "Packet.h"
namespace sim{
    struct BucketState{
        double r = std::numeric_limits<double>::infinity();
        std::queue<Packet> queue;
    };

    std::ostream& operator<<(std::ostream& out, const BucketState& s) {
        out <<"{r: "<< s.r
            <<" queue_size:"<<s.queue.size()
            <<"}";
        return out;
    }
    class Bucket : public cadmium::Atomic<BucketState>{
    public:
        constexpr static char NAME[] = "BUCKET";
        constexpr static double PROCESS_TIME = 0.01;
        cadmium::BigPort<Packet> resOut;
        cadmium::BigPort<Packet> reqIn;
        Bucket(const std::string& id): Atomic<BucketState>(id, BucketState()){
            reqIn = addInBigPort<Packet>("reqIn");
            resOut = addOutBigPort<Packet>("resOut");
        }
        void internalTransition(BucketState& s) const override {
            if(!s.queue.empty())
                s.queue.pop();
            s.r = s.queue.empty()?std::numeric_limits<double>::infinity():PROCESS_TIME;
        }

        void externalTransition(BucketState &s, double e) const override {
            s.queue.emplace(reqIn.get()->getBag().back());
            if(s.r == std::numeric_limits<double>::infinity()){
                s.r = PROCESS_TIME;
            }
            else{
                s.r = s.r - e;
            }
        }

        void output(const BucketState& s) const override {
            auto front = s.queue.front();
            if(front.type == PacketType::REQUEST){
                resOut->addMessage(front.creationTime, PacketType::RESPONSE, NAME, "BROWSER");
            }
            else{
                std::stringstream message;
                message<<"invalid packet: " << front;
                throw std::runtime_error(message.str());
            }
        }
        [[nodiscard]] double timeAdvance(const BucketState& s) const override {
            return s.r;
        }

    };


}
#endif //SIMPLE_WEBSITE_SIMULATION_BUCKET_H
