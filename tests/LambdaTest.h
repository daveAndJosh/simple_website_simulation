#pragma once
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include "BigIestream.hpp"
#include "Packet.h"
#include "../include/Lambda.h"
namespace test {
    using namespace sim;
    using namespace std;
    using namespace cadmium;

    struct LambdaTest: public cadmium::Coupled {
        LambdaTest() : Coupled("Test Harness") {
            auto lambda = addComponent<Lambda>(1);
            auto mockApiEndpoint= addComponent<BigIEStream<Packet>>("MockAPI","../tests/lambda_test_input/apiPort.txt");
            auto mockDb= addComponent<BigIEStream<Packet>>("MockDb","../tests/lambda_test_input/dbPort.txt");

            addIC(mockApiEndpoint->out, lambda->reqIn);
            addIC(mockDb->out, lambda->dbReceive);
        }
    };
}
