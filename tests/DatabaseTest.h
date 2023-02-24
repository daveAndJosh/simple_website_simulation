#pragma once
#include "../include/Database.h"
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

    struct DatabaseTest: public cadmium::Coupled {
        DatabaseTest() : Coupled("MockLambda") {
            auto database = addComponent<Database>("database");
            auto mock= addComponent<BigIEStream<Packet>>("Mock","../tests/DatabaseData.txt");

            addIC(mock->out, database->reqIn);
        }
    };
}

