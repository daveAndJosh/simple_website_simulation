//
// Created by dankvid on 2/23/23.
//

#pragma once
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include <set>
#include "BigIestream.hpp"
#include "Packet.h"
#include "../include/Switch.h"
namespace test {
using namespace sim;
using namespace std;
using namespace cadmium;

    struct SwitchTest: public cadmium::Coupled {

        SwitchTest() : Coupled("SwitchTest") {
            std::set<std::string> hostnames = {"host1","host2","host3"};
            auto zwitch= addComponent<Switch>(hostnames);
            auto host= addComponent<BigIEStream<Packet>>("Input","../tests/Switch_test_input/SwitchTestData.txt");

            addIC(host->out, zwitch->messageIn);
        }
    };
}
