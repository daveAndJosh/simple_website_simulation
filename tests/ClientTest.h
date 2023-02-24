#pragma once
#include "../include/Client.h"
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

    struct ClientTest: public cadmium::Coupled {
        ClientTest() : Coupled("MockCloud") {
            auto client = addComponent<Client>(5);
            auto mockApiEndpoint= addComponent<BigIEStream<Packet>>("MockAPI","../tests/client_test_input/apiPort.txt");
            auto mockWebHost= addComponent<BigIEStream<Packet>>("MockBucket","../tests/client_test_input/webPort.txt");

            addIC(mockApiEndpoint->out, client->apiInPort);
            addIC(mockWebHost->out, client->webInPort);
        }
    };
}
