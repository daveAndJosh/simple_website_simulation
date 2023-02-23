//
// Created by dankvid on 2/22/23.
//

#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_SWITCH_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_SWITCH_H
#include <cadmium/core/modeling/atomic.hpp>
#include <queue>
#include <limits>
#include <iostream>
#include <map>
#include "Packet.h"

namespace sim{
    struct SwitchState{
        std::queue<Packet> queue;
    };

    std::ostream& operator<<(std::ostream& out, const SwitchState& s) {
        out <<"{queue_size:"<<s.queue.size()
            <<"}";
        return out;
    }
    class Switch: public cadmium::Atomic<SwitchState>{
    public:
        constexpr static char NAME[] = "Switch";
        std::map<std::string, cadmium::BigPort<Packet>> messageOut;
        cadmium::BigPort<Packet> messageIn;
        Switch(const std::set<std::string>& outputNames): Atomic<SwitchState>(NAME, SwitchState()){
            for(const auto& name: outputNames){
                addOutPort(name);
            }

            messageIn = addInBigPort<Packet>("Input");

        }

        void addOutPort(const std::string& name){

            const auto& outPort= addOutBigPort<Packet>(name);
            messageOut.emplace(name, outPort);
        }

        void internalTransition(SwitchState& s) const override {
            if(!s.queue.empty())
                s.queue.pop();

        }

        void externalTransition(SwitchState &s, double e) const override {
            for(const auto& msg : messageIn->getBag()){
                s.queue.emplace(msg);
            }
        }

        void output(const SwitchState& s) const override {
            auto msg = s.queue.front();
            if(!messageOut.count(msg.dest)){
                throw std::runtime_error("invalid destination");
            }
            messageOut.at(msg.dest)->addMessage(msg);
        }

        [[nodiscard]] double timeAdvance(const SwitchState& s) const override {
            return s.queue.empty()?std::numeric_limits<double>::infinity():0;
        }

    };
}

#endif //MY_SERVERLESS_RECIPE_SITE_SIM_SWITCH_H