#include <iostream>
#include <memory>
#include "gossip_coupled.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/logger/csv_logger.hpp>

int main(int argc, char** argv) {
    std::string configFile = "./configs/gossip_neumann_50.json";
    if (argc > 1) {
        configFile = argv[1];
    }

    double simTime = 15.0;

    auto model = gossip_coupled("gossip_model", configFile);
    model.buildModel();

    auto coordinator = cadmium::Coordinator(model);
    auto logger = std::make_shared<cadmium::CSVLogger>("gossip_log.csv");

    coordinator.setLogger(logger);
    coordinator.start();
    coordinator.simulate(simTime);
    coordinator.stop();

    return 0;
}