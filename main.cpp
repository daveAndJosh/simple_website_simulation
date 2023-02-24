#include <cadmium/core/logger/csv.hpp>
#include <cadmium/core/simulation/root_coordinator.hpp>
#include "Internet.h"
using namespace sim;


void runWithOneExec(){

    auto model = std::make_shared<Internet>("Internet", 5,1);
    auto rootCoordinator = cadmium::RootCoordinator(model);
    auto logger = std::make_shared<cadmium::CSVLogger>("../logs/one.csv", ";");
    rootCoordinator.setLogger(logger);
    rootCoordinator.start();
    rootCoordinator.simulate(10000.0);
    rootCoordinator.stop();
}

void runWithManyExec(){

    auto model = std::make_shared<Internet>("Internet", 5,3);
    auto rootCoordinator = cadmium::RootCoordinator(model);
    auto logger = std::make_shared<cadmium::CSVLogger>("../logs/many.csv", ";");
    rootCoordinator.setLogger(logger);
    rootCoordinator.start();
    rootCoordinator.simulate(10000.0);
    rootCoordinator.stop();
}
int main() {
    runWithOneExec();
    runWithManyExec();
}
