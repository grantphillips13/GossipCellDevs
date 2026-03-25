// #ifndef GOSSIP_COUPLED_HPP
// #define GOSSIP_COUPLED_HPP

// #include <cadmium/modeling/celldevs/asymm/config.hpp>
// #include <cadmium/modeling/celldevs/asymm/coupled.hpp>
// #include "gossip_cell.hpp"

// using namespace cadmium::celldevs;

// inline std::shared_ptr<AsymmCell<GossipState, double>>
// addGossipCell(
//     const std::string& cellId,
//     const std::shared_ptr<const AsymmCellConfig<GossipState, double>>& config
// ) {
//     if (config->cellModel == "GossipCell") {
//         return std::make_shared<GossipCell>(cellId, config);
//     } else {
//         throw std::bad_typeid();
//     }
// }

// #endif // GOSSIP_COUPLED_HPP