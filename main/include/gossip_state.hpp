#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

struct gossip_state {
    bool knows;

    gossip_state() : knows(false) {}
};

inline bool operator!=(const gossip_state& a, const gossip_state& b) {
    return a.knows != b.knows;
}

inline std::ostream& operator<<(std::ostream& os, const gossip_state& s) {
    os << "<" << (s.knows ? 1 : 0) << ">";
    return os;
}

inline void from_json(const nlohmann::json& j, gossip_state& s) {
    j.at("knows").get_to(s.knows);
}