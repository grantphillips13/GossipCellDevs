# Gossip Spread — Cell-DEVS Cadmium Simulation

---

## Overview

This project simulates the emergent spread of information (gossip) through a population using the **Cell-DEVS formalism**

Individuals are modeled as cells on a 10×10 grid. Each cell represents a person who either **knows** (`1`) or **does not know** (`0`) a piece of information. A single source cell starts as informed, and information propagates outward to neighbors according to a configurable spread probability. Informed cells may also forget the information according to a configurable forget probability.

The model supports both **Moore** and **Von Neumann** neighborhoods and is parameterized to allow experimentation across different spread and forget rates.

Each cell tracks a single state variable:

| Variable | Values | Meaning |
|---|---|---|
| `knows` | `0` / `1` | Cell is ignorant / Cell is informed |

**Spread logic:** An uninformed cell receives information from each informed neighbor independently. The compound probability of learning from *k* informed neighbors is:

$$p_{\text{spread}} = 1 - (1 - p)^k$$

where $p$ is the per-neighbor `spread_prob`. An informed cell forgets with probability `forget_prob` each step.

---

## File Organization

```
GossipCellDevs-1/
├── main/
│   ├── main.cpp                                    # Simulation entry point
│   ├── CMakeLists.txt                              # CMake config for main target
│   └── include/
│       ├── gossip_cell.hpp                         # GossipCell: local computation function (tau)
│       └── gossip_state.hpp                        # gossip_state struct, JSON parsing
├── config/
│   ├── gossip_config.json                          # Default config (Moore, spread=0.5, forget=0.1)
│   ├── gossip_moore_050_config.json                # Moore neighborhood, spread_prob=0.50
│   ├── gossip_moore_075_config.json                # Moore neighborhood, spread_prob=0.75
│   ├── gossip_neumann_025_config.json              # Von Neumann neighborhood, spread_prob=0.25
│   ├── gossip_neumann_050_config.json              # Von Neumann neighborhood, spread_prob=0.50
│   ├── gossip_neumann_075_config.json              # Von Neumann neighborhood, spread_prob=0.75
│   ├── gossip_test_fullspread_config.json          # Test: guaranteed spread, no forgetting
│   ├── gossip_test_no_spread_config.json           # Test: no spread, no forgetting
│   ├── gossip_moore_050_visualization_config.json  # Viewer config for Moore 0.50
│   ├── gossip_moore_075_viewer_config.json         # Viewer config for Moore 0.75
│   ├── gossip_neumann_025_visualization_config.json
│   ├── gossip_neumann_050_visualization_config.json
│   ├── gossip_neumann_075_visualization_config.json
│   ├── gossip_test_fullspread_visualization_config.json
│   ├── gossip_test_nospread_visualization_config.json
│   └── gossipVisualization_config.json             # General visualization config
├── bin/
│   └── Gossip_Model                                # Compiled simulation executable
├── build/                                          # CMake build artifacts (generated)
├── gossip_log.csv                                  # Simulation output log
├── CMakeLists.txt                                  # Top-level CMake build configuration
├── build.sh                                        # Build script
└── README.md
```

---

## Prerequisites

- **C++17** compatible compiler (e.g., `g++`)
- **CMake** 3.16 or later
- **Cadmium v2** — header-only DEVS simulation library (includes nlohmann/json)

Cadmium must be available either:
- At `../cadmium_v2/` relative to this project root, **or**
- Via the `CADMIUM` environment variable pointing to Cadmium's `include/` directory

To set the environment variable:
```bash
export CADMIUM=/path/to/cadmium_v2/include
```

---

## Compilation Instructions

Use the provided build script to clean, configure, and compile:

```bash
source build.sh
```

This will:
1. Verify the Cadmium library path
2. Remove any previous `build/` directory
3. Run `cmake ..` to configure the project
4. Run `make` to compile the `Gossip_Model` executable into `bin/`

Alternatively, build manually:
```bash
mkdir -p build && cd build
cmake ..
make
cd ..
```

---

## Testing Instructions

Run a scenario by passing a config file path and an optional simulation time (default: 100):

```bash
./bin/Gossip_Model config/<scenario_config>.json [sim_time]
```

**Run all scenarios:**

```bash
# Moore neighborhood
./bin/Gossip_Model config/gossip_moore_050_config.json 100
./bin/Gossip_Model config/gossip_moore_075_config.json 100

# Von Neumann neighborhood
./bin/Gossip_Model config/gossip_neumann_025_config.json 100
./bin/Gossip_Model config/gossip_neumann_050_config.json 100
./bin/Gossip_Model config/gossip_neumann_075_config.json 100

# Edge-case tests
./bin/Gossip_Model config/gossip_test_fullspread_config.json 100
./bin/Gossip_Model config/gossip_test_no_spread_config.json 100
```

Each run produces a `gossip_log.csv` file in the working directory containing the state of every cell at each simulation time step.

**Scenario parameter summary:**

| Config File | Neighborhood | `spread_prob` | `forget_prob` |
|---|---|---|---|
| `gossip_moore_050_config.json` | Moore | 0.50 | 0.10 |
| `gossip_moore_075_config.json` | Moore | 0.75 | 0.10 |
| `gossip_neumann_025_config.json` | Von Neumann | 0.25 | 0.10 |
| `gossip_neumann_050_config.json` | Von Neumann | 0.50 | 0.10 |
| `gossip_neumann_075_config.json` | Von Neumann | 0.75 | 0.10 |
| `gossip_test_fullspread_config.json` | Moore | 1.00 | 0.00 |
| `gossip_test_no_spread_config.json` | Moore | 0.00 | 0.00 |

**Visualizing results** using the [Cell-DEVS Web Viewer](https://devssim.carleton.ca/cell-devs-viewer/):
1. Load the corresponding visualization config JSON file (e.g., `gossip_moore_075_viewer_config.json`)
2. Load the `gossip_log.csv` output file
3. Step through or animate the simulation to observe the information wave spreading from the source cell at `[5, 5]`

**Color coding for the `knows` field:**

| State | Color |
|---|---|
| Ignorant (`0`) | Light grey `rgb(220, 220, 220)` |
| Informed (`1`) | Red `rgb(255, 0, 0)` |
