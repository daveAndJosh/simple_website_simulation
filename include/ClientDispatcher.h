#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_CLIENTDISPATCHER_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_CLIENTDISPATCHER_H

#include <cadmium/core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include <map>

#include "Packet.h"
namespace sim{
    struct ClientDispatchState{
        double r = std::numeric_limits<double>::infinity();
        std::queue<Packet> queue;
    };

    std::ostream& operator<<(std::ostream& out, const ClientDispatchState& s) {
        out <<"{r: "<< s.r
            <<" queue_size:"<<s.queue.size()
            <<"}";
        return out;
    }
    class ClientDispatch : public cadmium::Atomic<ClientDispatchState>{
    public:
        constexpr static char NAME[] = "CLIENT_DISPATCHER";
        constexpr static double PROCESS_TIME = 0.001;
        cadmium::BigPort<Packet> pageOut;
        cadmium::BigPort<Packet> apiOut;
        cadmium::BigPort<Packet> in;
        ClientDispatch(const std::string& id): Atomic<ClientDispatchState>(id, ClientDispatchState()){
            in = addInBigPort<Packet>("in");
            pageOut = addOutBigPort<Packet>("pageOut");
            apiOut = addOutBigPort<Packet>("apiOut");
        }
        void internalTransition(ClientDispatchState& s) const override {
            if(!s.queue.empty())
                s.queue.pop();
            s.r = s.queue.empty()?std::numeric_limits<double>::infinity():PROCESS_TIME;
        }

        void externalTransition(ClientDispatchState &s, double e) const override {
            if(s.r == std::numeric_limits<double>::infinity()){
                s.r = PROCESS_TIME;
            }
            else{
                s.r = s.r - e;
            }
            for(const auto& val : in.get()->getBag()){
                s.queue.emplace(val);
            }
            in->clear();
        }

        void output(const ClientDispatchState& s) const override {
            auto front = s.queue.front();
            if(front.dest == "PAGE"){
                pageOut->addMessage(front);
            }
            else if(front.type == PacketType::RESPONSE&& front.dest == "BROWSER"){ // change packet to a request and sends it
                apiOut->addMessage(front.creationTime, PacketType::REQUEST, "BROWSER", "API");
            }
            else{
                std::stringstream message;
                message<<"invalid packet: " << front;
                throw std::runtime_error(message.str());
            }
        }
        [[nodiscard]] double timeAdvance(const ClientDispatchState& s) const override {
            return s.r;
        }

    };


}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_CLIENTDISPATCHER_H
