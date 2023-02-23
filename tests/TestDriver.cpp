//
// Created by dankvid on 2/23/23.
//
#include <iostream>
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include <cadmium/lib/iestream.hpp>
#include "GeneratorTest.h"
#include "CounterTest.h"

using namespace std;
using namespace sim;

int main(){
    std::vector<std::pair<std::string, shared_ptr<cadmium::Coupled>>> tests{
        std::make_pair("generatorTest", std::make_shared<test::GeneratorTest>()),
        std::make_pair("counterTest", std::make_shared<test::CounterTest>())
    };

    for(const auto& test: tests){
        const auto& file = test.first;
        const auto& model = test.second;
        auto rootCoordinator = cadmium::RootCoordinator(model);
        auto logger = std::make_shared<cadmium::CSVLogger>("../logs/"+file+".csv", ";");
        rootCoordinator.setLogger(logger);
        rootCoordinator.start();
        rootCoordinator.simulate(10000.0);
        rootCoordinator.stop();
    }
    //auto model = std::make_shared<test::GeneratorTest>();
    //auto model = std::make_shared<test::CounterTest>();


}