#pragma once
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include "BigIestream.hpp"
#include "Packet.h"
#include "../include/LambdaDispatcher.h"
namespace test {
    using namespace sim;
    using namespace std;
    using namespace cadmium;

    struct LambdaDispatcherTest: public cadmium::Coupled {
        LambdaDispatcherTest() : Coupled("Test Harness") {
            set <std::string> execNames = {"exec1", "exec2"};
            auto dispatcher = addComponent<LambdaDispatcher>(execNames);
            auto mockIsReady = addComponent<BigIEStream<std::string>>("MockIsReady",
                                                                 "../tests/lambda_dispatcher_test_input/isReadyPort.txt");
            auto mockQueueIn = addComponent<BigIEStream<Packet>>("MockQueueIn",
                                                                 "../tests/lambda_dispatcher_test_input/inPort.txt");

            addIC(mockQueueIn->out, dispatcher->queueIn);
            addIC(mockIsReady->out, dispatcher->isReadyPort);
        }
    };
}
