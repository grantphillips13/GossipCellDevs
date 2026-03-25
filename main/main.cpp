#include "nlohmann/json.hpp"
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <string>
#include "include/gossip_cell.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace cadmium::celldevs;
using namespace cadmium;

std::shared_ptr<GridCell<gossip_state, double>> addGridCell(
    const coordinates & cellId,
    const std::shared_ptr<const GridCellConfig<gossip_state, double>>& cellConfig
) {
    auto cellModel = cellConfig->cellModel;

    if (cellModel == "gossip") {
        return std::make_shared<gossip_cell>(cellId, cellConfig);
    } else {
        throw std::bad_typeid();
    }
}

void removeSepLineIfPresent(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::string firstLine;
    std::getline(in, firstLine);

    if (firstLine != "sep=;") {
        return; // nothing to fix
    }

    std::ostringstream remaining;
    remaining << in.rdbuf();
    in.close();

    std::ofstream out(filename, std::ios::trunc);
    out << remaining.str();
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cout << "Program used with wrong parameters. The program must be invoked as follows:";
        std::cout << argv[0] << " SCENARIO_CONFIG.json [MAX_SIMULATION_TIME (default: 100)]" << std::endl;
        return -1;
    }

    std::string configFilePath = argv[1];
    double simTime = (argc > 2) ? std::stod(argv[2]) : 100.0;

    auto model = std::make_shared<GridCellDEVSCoupled<gossip_state, double>>(
        "gossip",
        addGridCell,
        configFilePath
    );
    model->buildModel();

    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<CSVLogger>("gossip_log.csv", ";");

    rootCoordinator.start();
    rootCoordinator.simulate(simTime);
    rootCoordinator.stop();

    removeSepLineIfPresent("gossip_log.csv");
}