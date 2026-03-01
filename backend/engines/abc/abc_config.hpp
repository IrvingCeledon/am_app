#pragma once
#include "../../evo_core/config_base.hpp"

struct ABCConfig : EvoConfigBase {
    size_t limit{100}; 
    bool use_gbest{false};
};
