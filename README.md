# Gossip Spread — Cell-DEVS Cadmium Simulation

---

## Overview

This project simulates the emergent spread of information (gossip) through a population using the **Cell-DEVS formalism** implemented in **Cadmium**.

Individuals are modeled as cells on a 10×10 grid. Each cell represents a person who either knows (`1`) or does not know (`0`) a piece of information. A single source cell starts informed at `[5, 5]`, then information propagates to neighbors according to `spread_prob`. Informed cells may forget according to `forget_prob`.

The model supports both **Moore** and **Von Neumann** neighborhoods.

Each cell tracks a single state variable:

| Variable | Values | Meaning |
|---|---|---|
| `knows` | `0` / `1` | Cell is ignorant / Cell is informed |

Spread probability from `k` informed neighbors is:

$$p_{\text{spread}} = 1 - (1 - p)^k$$

where $p$ is the per-neighbor `spread_prob`.

---

## File Organization

```
GossipCellDevs-1/
├── main/
│   ├── main.cpp                             # Simulation entry point
│   ├── CMakeLists.txt                       # Main target build config
│   └── include/
│       ├── gossip_cell.hpp                  # Cell local transition logic
│       └── gossip_state.hpp                 # Cell state definition + JSON parsing
├── config/
│   ├── gossip_moore_025_config.json
│   ├── gossip_moore_050_config.json
│   ├── gossip_moore_075_config.json
│   ├── gossip_neumann_025_config.json
│   ├── gossip_neumann_050_config.json
│   ├── gossip_neumann_075_config.json
│   ├── gossip_test_fullspread_config.json
│   └── gossip_test_nospread_config.json
├── logs/
│   ├── moore_25.csv
│   ├── moore_50.csv
│   ├── moore_75.csv
│   ├── neumann_25.csv
│   ├── neumann_50.csv
│   ├── neumann_75.csv
│   ├── full_spread.csv
│   └── no_spread.csv
├── videos/                                  # Recorded simulation videos
├── bin/
│   └── Gossip_Model                         # Compiled executable
├── build/                                   # Generated CMake build artifacts
├── CMakeLists.txt                           # Top-level CMake config
├── build.sh                                 # Build script
└── README.md
```

---

## Prerequisites

- **C++17** compatible compiler (e.g., `g++`)
- **CMake** 3.16 or later
- **Cadmium v2** (header-only)

Cadmium must be available at either:
- `../cadmium_v2/` relative to this project, or
- the `CADMIUM` environment variable pointing to Cadmium `include/`

Example:

```bash
export CADMIUM=/path/to/cadmium_v2/include
```

---

## Compilation Instructions

Build with the provided script:

```bash
source build.sh
```

This script:
1. Verifies the Cadmium include path
2. Recreates `build/`
3. Runs `cmake ..`
4. Runs `make`

Manual build alternative:

```bash
mkdir -p build && cd build
cmake ..
make
cd ..
```

---

## Testing Instructions

Run a scenario with optional simulation time (default: 100):

```bash
./bin/Gossip_Model config/<scenario_config>.json [sim_time]
```

Run all scenarios:

```bash
./bin/Gossip_Model config/gossip_moore_025_config.json 100
./bin/Gossip_Model config/gossip_moore_050_config.json 100
./bin/Gossip_Model config/gossip_moore_075_config.json 100
./bin/Gossip_Model config/gossip_neumann_025_config.json 100
./bin/Gossip_Model config/gossip_neumann_050_config.json 100
./bin/Gossip_Model config/gossip_neumann_075_config.json 100
./bin/Gossip_Model config/gossip_test_fullspread_config.json 100
./bin/Gossip_Model config/gossip_test_nospread_config.json 100
```

Each run produces `gossip_log.csv` in the working directory.

---

## Scenario Summary

| Config File | Neighborhood | `spread_prob` | `forget_prob` |
|---|---|---|---|
| `gossip_moore_025_config.json` | Moore | 0.25 | 0.10 |
| `gossip_moore_050_config.json` | Moore | 0.50 | 0.10 |
| `gossip_moore_075_config.json` | Moore | 0.75 | 0.10 |
| `gossip_neumann_025_config.json` | Von Neumann | 0.25 | 0.10 |
| `gossip_neumann_050_config.json` | Von Neumann | 0.50 | 0.10 |
| `gossip_neumann_075_config.json` | Von Neumann | 0.75 | 0.10 |
| `gossip_test_fullspread_config.json` | Moore | 1.00 | 0.00 |
| `gossip_test_nospread_config.json` | Moore | 0.00 | 0.00 |

---

## Visualization

Use the [Cell-DEVS Web Viewer](https://devssim.carleton.ca/cell-devs-viewer/):
1. Load one of the `config/*.json` files
2. Load the generated `gossip_log.csv`
3. Step/animate to observe spread dynamics

Viewer colors for `knows`:

| State | Color |
|---|---|
| `0` (ignorant) | Light gray `rgb(220, 220, 220)` |
| `1` (informed) | Red `rgb(255, 0, 0)` |
