//
// Created by dankvid on 2/23/23.
//

#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_GENERATORTEST_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_GENERATORTEST_H

#include "../include/ClientGenerator.h"
#include <cadmium/core/modeling/coupled.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>

#include "Packet.h"
namespace test{
using namespace sim;
using namespace std;
using namespace cadmium;

    struct GeneratorTest : public cadmium::Coupled {

        /**
         * Client
         * @param period the period of how often to send website request messages
         */
        GeneratorTest(): Coupled("Client") {
            auto generator = addComponent<ClientGenerator>("ClientGenerator", 5);
        }
    };
}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_GENERATORTEST_H
