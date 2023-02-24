//
// Created by dankvid on 2/23/23.
//
#include <iostream>
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include "GeneratorTest.h"
#include "CounterTest.h"
#include "ClientDispatcherTest.h"
#include "ClientTest.h"
#include "ExecutorTest.h"
#include "APIGatewayTest.h"
#include "BucketTest.h"
#include "DatabaseTest.h"
#include "LambdaDispatcherTest.h"
#include "SwitchTest.h"
#include "LambdaTest.h"
#include "CloudTest.h"
using namespace std;
using namespace sim;

int main(){
    std::vector<std::pair<std::string, shared_ptr<cadmium::Coupled>>> tests{
        std::make_pair("generatorTest", std::make_shared<test::GeneratorTest>()),
        std::make_pair("counterTest", std::make_shared<test::CounterTest>()),
        std::make_pair("ClientDispatcherTest", std::make_shared<test::ClientDispatchTest>()),
        std::make_pair("ClientTest", std::make_shared<test::ClientTest>()),
        std::make_pair("ExecutorTest", std::make_shared<test::ExecutorTest>()),
        std::make_pair("ApiGatewayTest",std::make_shared<test::APIGatewayTest>()),
        std::make_pair("BucketTest",std::make_shared<test::BucketTest>()),
        std::make_pair("DatabaseTest",std::make_shared<test::DatabaseTest>()),
        std::make_pair("LambdaDispatcherTest",std::make_shared<test::LambdaDispatcherTest>()),
        std::make_pair("SwitchTest",std::make_shared<test::SwitchTest>()),
        std::make_pair("LambdaTest", std::make_shared<test::LambdaTest>()),
        std::make_pair("CloudTest", std::make_shared<test::CloudTest>()),
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