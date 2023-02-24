#pragma once
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include "BigIestream.hpp"
#include "Packet.h"
#include "../include/APIGateway.h"
namespace test {
    using namespace sim;
    using namespace std;
    using namespace cadmium;

    struct APIGatewayTest: public cadmium::Coupled {
        APIGatewayTest() : Coupled("MockCloud") {
            auto apiGateway = addComponent<APIGateway>("apiGateway");
            auto mockLambda= addComponent<BigIEStream<Packet>>("MockLambda","../tests/api_gateway_test_input/APIResponseInputData.txt");
            auto mockClient= addComponent<BigIEStream<Packet>>("MockClient","../tests/api_gateway_test_input/APIRequestInputData.txt");


            addIC(mockClient->out, apiGateway->reqIn);
            addIC(mockLambda->out, apiGateway->resIn);
        }
    };
}

