#pragma once
#include "../../evo_core/config_base.hpp"

struct PSOConfig : EvoConfigBase {
    double w{0.7298};     // Inertia weight
    double c1{1.49618};   // Cognitive component
    double c2{1.49618};   // Social component
};
