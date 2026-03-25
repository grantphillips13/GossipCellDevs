#ifndef GOSSIP_CELL_HPP
#define GOSSIP_CELL_HPP

#include <cmath>
#include <random>
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include <cadmium/modeling/celldevs/grid/config.hpp>
#include "gossip_state.hpp"

using namespace cadmium::celldevs;

class gossip_cell : public GridCell<gossip_state, double> {
public:
    gossip_cell(
        const std::vector<int>& id,
        const std::shared_ptr<const GridCellConfig<gossip_state, double>>& config
    )
    : GridCell<gossip_state, double>(id, config), dist_(0.0, 1.0)
    {
        config->rawCellConfig.at("spread_prob").get_to(spread_prob_);
        config->rawCellConfig.at("forget_prob").get_to(forget_prob_);

        unsigned seed = 12345;
        for (int v : id) {
            seed = seed * 31 + static_cast<unsigned>(v + 1000);
        }
        gen_.seed(seed);
    }

    [[nodiscard]] gossip_state localComputation(
        gossip_state state,
        const std::unordered_map<std::vector<int>, NeighborData<gossip_state, double>>& neighborhood
    ) const override {
        int active_neighbors = 0;

        for (const auto& [neighborId, neighborData] : neighborhood) {
            auto nState = neighborData.state;
            if (nState->knows) {
                active_neighbors++;
            }
        }

        if (state.knows) {
            active_neighbors--; // self is included in Moore neighborhood
        }

        double roll = dist_(gen_);

        if (!state.knows) {
            if (active_neighbors > 0) {
                double p_spread = 1.0 - std::pow(1.0 - spread_prob_, active_neighbors);
                if (roll < p_spread) {
                    state.knows = true;
                }
            }
        } else {
            if (roll < forget_prob_) {
                state.knows = false;
            }
        }

        return state;
    }

    [[nodiscard]] double outputDelay(const gossip_state& state) const override {
        return 1.0;
    }

private:
    double spread_prob_;
    double forget_prob_;

    mutable std::mt19937 gen_;
    mutable std::uniform_real_distribution<double> dist_;
};

#endif // GOSSIP_CELL_HPP