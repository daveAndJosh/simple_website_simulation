#pragma once
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include "BigIestream.hpp"
#include "Packet.h"
#include "../include/Cloud.h"
namespace test {
    using namespace sim;
    using namespace std;
    using namespace cadmium;

    struct CloudTest: public cadmium::Coupled {
        CloudTest() : Coupled("Test Harness") {
            auto cloud = addComponent<Cloud>();
            auto mockApiRequester= addComponent<BigIEStream<Packet>>("MockAPI","../tests/cloud_test_input/apiPort.txt");
            auto mockWebRequester= addComponent<BigIEStream<Packet>>("MockBucket","../tests/cloud_test_input/webPort.txt");

            addIC(mockApiRequester->out, cloud->apiRequestFromClient);
            addIC(mockWebRequester->out, cloud->webRequestFromClient);
        }
    };
}
