#pragma once
#include "../include/Executor.h"
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

    struct ExecutorTest: public cadmium::Coupled {
        ExecutorTest() : Coupled("TestHarness") {
            auto executor= addComponent<sim::Exector>("Executor");
            auto mockApiEndpoint= addComponent<BigIEStream<Packet>>("MockAPI","../tests/executor_test_input/apiPort.txt");
            auto mockDb= addComponent<BigIEStream<Packet>>("MockDB","../tests/executor_test_input/dbPort.txt");

            addIC(mockApiEndpoint->out, executor->reqIn);
            addIC(mockDb->out, executor->dbInPort);
        }
    };
}
