#pragma once
#include "../include/ClientDispatcher.h"
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

    struct ClientDispatchTest: public cadmium::Coupled {
        ClientDispatchTest() : Coupled("MockClient") {
            auto dispatcher = addComponent<ClientDispatch>("dispatcher");
            auto mock= addComponent<BigIEStream<Packet>>("Mock","../tests/ClientDispatchData.txt");

            addIC(mock->out, dispatcher->in);
        }
    };
}
