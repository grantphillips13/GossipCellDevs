#ifndef GOSSIP_STATE_HPP
#define GOSSIP_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>

struct gossip_state {
    int knows;

    gossip_state() : knows(0) {}
};

inline std::ostream& operator<<(std::ostream& os, const gossip_state& x) {
    os << x.knows;
    return os;
}

inline bool operator!=(const gossip_state& x, const gossip_state& y) {
    return x.knows != y.knows;
}

inline void from_json(const nlohmann::json& j, gossip_state& s) {
    j.at("knows").get_to(s.knows);
}

#endif