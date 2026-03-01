#pragma once

#include <random>
#include "../../evo_core/population.hpp"
#include "../../evo_core/binary_individual.hpp"
#include "ga_config.hpp"

class GAEngine
{
private:
    std::mt19937 rng{ std::random_device{}() };
    GAConfig configuration;
    Population<BinaryIndividual> pop;

    // Algorithm's intern methods
    uint32_t randGene();
    void decode_individual(BinaryIndividual&);

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
    explicit GAEngine(const GAConfig& config) 
        : configuration(config),
          pop(config.populationSize)
    {}

    // Main entry point
    RunResult run();
};
