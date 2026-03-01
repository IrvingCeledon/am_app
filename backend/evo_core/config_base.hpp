#pragma once

#include <cstddef>
#include "types.hpp"

// ==========================================
// Base configuration for ANY evolutionary engine
// ==========================================

struct EvoConfigBase
{
    size_t generations{};
    size_t populationSize{};

    Domains domains;   // The search space
    Fitness fitness;   // The objective function to be optimized
    
    // ==========================================
    // Stopping Criteria (Universal Failsafes)
    // ==========================================
    bool use_target{false};
    double target_fitness{0.0001};

    bool use_stagnation{false};
    size_t stagnation_patience{20};
    
    // Nivel de logging exigido por Python
    LogLevel verbosity{LogLevel::INFO_LVL};
};
