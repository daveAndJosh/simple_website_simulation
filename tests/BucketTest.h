#pragma once
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include "BigIestream.hpp"
#include "Packet.h"
#include "../include/Bucket.h"
namespace test {
    using namespace sim;
    using namespace std;
    using namespace cadmium;

    struct BucketTest: public cadmium::Coupled {
        BucketTest() : Coupled("Test Harness") {
            auto bucket = addComponent<Bucket>("bucket");
            auto mock= addComponent<BigIEStream<Packet>>("Mock","../tests/BucketData.txt");

            addIC(mock->out, bucket->reqIn);
        }
    };
}

