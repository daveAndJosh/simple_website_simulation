//
// Created by dankvid on 2/23/23.
//

#pragma once
#include "../include/PacketCounter.h"
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include "BigIestream.hpp"
#include "Packet.h"
namespace test {
using namespace sim;
using namespace std;
using namespace cadmium;

    struct CounterTest : public cadmium::Coupled {

        /**
         * Client
         * @param period the period of how often to send website request messages
         */
        CounterTest() : Coupled("Client") {
            auto counter = addComponent<PacketCounter>();
            auto parser = addComponent<BigIEStream<Packet>>("Mock","../tests/CounterTestData.txt");

            addIC(parser->out, counter->packetInputPort);
        }
    };
}
