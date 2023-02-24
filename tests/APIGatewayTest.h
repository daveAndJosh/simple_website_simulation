#pragma once
#include "../include/APIGateway.h"
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

    struct APIGatewayTest: public cadmium::Coupled {
        APIGatewayTest() : Coupled("MockCloud") {
            auto apiGateway = addComponent<APIGateway>("apiGateway");
            auto mockLambda= addComponent<BigIEStream<Packet>>("MockLambda","../tests/APIResponseInputData.txt");
            auto mockClient= addComponent<BigIEStream<Packet>>("MockClient","../tests/APIRequestInputData.txt");


            addIC(mockClient->out, apiGateway->reqIn);
            addIC(mockLambda->out, apiGateway->resIn);
        }
    };
}

