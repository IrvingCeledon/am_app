#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "../../evo_core/types.hpp"
#include "../../evo_core/population.hpp"
#include "bfoa_config.hpp"
#include "bacterium.hpp"

class BFOAEngine
{
private:
    BFOAConfig configuration;
    Population<Bacterium> pop;
    std::mt19937 rng{ std::random_device{}() };

    Bacterium global_best;

    // Core BFOA phases mapped from the canonical implementation
    void initialize();
    double calculate_swarming(size_t bacteria_index);
    void chemotaxis();
    void reproduction();
    void reorientation();

    // Helpers
    double evaluate_fitness(const Genome& g) const;
    void clamp_bacterium(Bacterium& b);
    void save_history(std::vector<Genome>& target);
    void push_log(RunResult& res, LogLevel lvl, const std::string& msg);

public:
    explicit BFOAEngine(const BFOAConfig& config)
    : configuration(config), pop(config.populationSize) {}

    // Main execution loop
    RunResult run();
};
