//
// Created by dankvid on 2/22/23.
//

#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_PACKETCOUNTER_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_PACKETCOUNTER_H

#include <cadmium/core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

#include "Packet.h"
namespace sim{
    struct CounterState{
        unsigned long counter = 0;
    };

    std::ostream& operator<<(std::ostream& out, const CounterState& s) {
        out << "{packets_received: "<<s.counter<<"}";
        return out;
    }
    class PacketCounter : public cadmium::Atomic<CounterState>{
    private:
    public:
        constexpr static char NAME[] = "COUNTER";
        cadmium::BigPort<Packet> packetInputPort;
        PacketCounter(): Atomic<CounterState>(NAME, CounterState()){
            packetInputPort = addInBigPort<Packet>("PacketInputPort");
        }
        void internalTransition(CounterState& s) const override {
        }

        void externalTransition(CounterState &s, double e) const override {
            s.counter+=packetInputPort->getBag().size();
        }

        void output(const CounterState& s) const override {

        }
        [[nodiscard]] double timeAdvance(const CounterState& s) const override {
            return std::numeric_limits<double>::infinity();
        }

    };


}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_PACKETCOUNTER_H
