#include <cadmium/core/logger/csv.hpp>
#include <cadmium/core/simulation/root_coordinator.hpp>
#include "Internet.h"
using namespace sim;

int main() {
    auto model = std::make_shared<Internet>("Internet", 5);
    auto rootCoordinator = cadmium::RootCoordinator(model);
    auto logger = std::make_shared<cadmium::CSVLogger>("log.csv", ";");
    rootCoordinator.setLogger(logger);
    rootCoordinator.start();
    rootCoordinator.simulate(10000.0);
    rootCoordinator.stop();
}
