#pragma once
#include <string>
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include "gossip_cell.hpp"

using namespace cadmium;
using namespace std;

class gossip_coupled : public GridCellDEVSCoupled<gossip_state, double> {
public:
    gossip_coupled(const string& id, const string& configFilePath)
        : GridCellDEVSCoupled<gossip_state, double>(id, configFilePath) {}

    void addCell(const std::vector<int>& cellId,
                 const std::shared_ptr<const GridCellConfig<gossip_state, double>>& cellConfig) const override {
        addComponent(gossip_cell(cellId, cellConfig));
    }
};