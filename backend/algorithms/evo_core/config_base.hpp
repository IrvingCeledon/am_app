#pragma once

#include <cstddef>
#include "types.hpp"

// ==========================================
// Base configuration for any evolutionary algorithm
// ==========================================

struct EvoConfigBase
{
    size_t generations{};
    size_t populationSize{};

    Domains domains;   // search space
    Fitness fitness;   // objective function
    Rates rates;       // selection/crossover/mutation
};
