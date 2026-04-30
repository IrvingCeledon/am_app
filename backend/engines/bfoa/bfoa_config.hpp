#pragma once
#include "../../evo_core/config_base.hpp"

struct BFOAConfig : EvoConfigBase {
    // We ignore the 'generations' variable from EvoConfigBase
    // as BFOA uses a specific nested loop architecture.
    size_t chemotactic_steps{20};
    size_t reproduction_steps{4};
    size_t elimination_dispersal_steps{2};
    size_t run_length{4};

    double step_size{0.1};
    double elimination_rate{0.25};

    // Cell-to-cell attractant and repellant parameters (Swarming)
    double d_attract{0.1};
    double w_attract{0.2};
    double h_repel{0.1};
    double w_repel{10.0};
};
