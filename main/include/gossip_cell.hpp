#pragma once
#include <random>
#include <cmath>
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include "gossip_state.hpp"

using namespace cadmium;
using namespace std;

class gossip_cell : public GridCell<gossip_state, double> {
private:
    double spread_prob;
    double forget_prob;

    // Simple RNG per cell
    mutable std::mt19937 gen;
    mutable std::uniform_real_distribution<double> dist;

public:
    gossip_cell(const std::vector<int>& id,
                const std::shared_ptr<const GridCellConfig<gossip_state, double>>& config)
        : GridCell<gossip_state, double>(id, config),
          spread_prob(0.5),
          forget_prob(0.1),
          dist(0.0, 1.0) {

        // Read custom config values from JSON
        if (config->rawCellConfig.contains("spread_prob")) {
            config->rawCellConfig.at("spread_prob").get_to(spread_prob);
        }
        if (config->rawCellConfig.contains("forget_prob")) {
            config->rawCellConfig.at("forget_prob").get_to(forget_prob);
        }

        // deterministic-ish seed per cell
        unsigned seed = 12345;
        for (int v : id) seed = seed * 31 + static_cast<unsigned>(v + 1000);
        gen.seed(seed);
    }

    gossip_state localComputation(
        gossip_state state,
        const std::unordered_map<std::vector<int>, NeighborData<gossip_state, double>>& neighborhood
    ) const override {

        int active_neighbors = 0;

        for (const auto& [neighbor_id, neighbor_data] : neighborhood) {
            auto nstate = neighbor_data.state;
            if (nstate->knows) {
                active_neighbors++;
            }
        }

        // remove self if your neighborhood includes (0,0)
        if (state.knows) {
            active_neighbors--;
        }

        double roll = dist(gen);

        if (!state.knows) {
            if (active_neighbors > 0) {
                double p_become = 1.0 - std::pow(1.0 - spread_prob, active_neighbors);
                if (roll < p_become) {
                    state.knows = true;
                }
            }
        } else {
            if (roll < forget_prob) {
                state.knows = false;
            }
        }

        return state;
    }

    double outputDelay(const gossip_state& state) const override {
        return 1.0;
    }
};