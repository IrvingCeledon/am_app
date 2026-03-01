#pragma once

#include <vector>
#include <functional>

// Real-valued phenotype (R^n)
using Genome = std::vector<double>;

// Universal objective function: f : R^n -> R
using Fitness = std::function<double(const Genome&)>;

struct Range {
    double min{};
    double max{};
};

struct Domains {
    // One range per dimension
    std::vector<Range> ranges;

    size_t dimension() const noexcept {
        return ranges.size();
    }

    const Range& operator[](size_t i) const noexcept {
        return ranges[i];
    }
};

struct Rates {
    double selection{}; 
    double crossover{};
    double mutation{};
};

// ===============================
// Run results (shared by GA/CGA)
// ===============================

struct RunResult {
    std::vector<double> bestFitnesses;

    // population history in phenotype space
    std::vector<Genome> initialPopulation;
    std::vector<Genome> midPopulation;
    std::vector<Genome> finalPopulation;
};
