#pragma once

#include <random>
#include <vector>
#include "../../evo_core/population.hpp"
#include "../../evo_core/real_individual.hpp"
#include "cga_config.hpp"

class CGAEngine
{
private:
    std::mt19937 rng{ std::random_device{}() };
    CGAConfig configuration;
    Population<RealIndividual> pop;

    // Internal helper to keep values ​​within limits
    void clamp_individual(RealIndividual& ind);

    // Generational phases
    void initialize();
    void evaluate();
    void selection();
    void crossover();
    void mutate();

    // Helper to save different populations results
    void save_history(std::vector<Genome>&);
    bool check_stopping_criteria(size_t&, double&, double);

public: 
    explicit CGAEngine(const CGAConfig& config) 
        : configuration(config),
          pop(config.populationSize)
    {}

    // Main entry point
    RunResult run();
};
