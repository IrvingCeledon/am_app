#pragma once
#include "../../evo_core/config_base.hpp"

struct DEConfig : EvoConfigBase {
    double F{0.8};  // Differential weight
    double CR{0.9}; // Crossover probability
};
