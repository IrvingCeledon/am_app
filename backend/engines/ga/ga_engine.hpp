#pragma once

#include <random>
#include "../../evo_core/binary_individual.hpp"
#include "../genetic_engine_base.hpp"
#include "ga_config.hpp"

class GAEngine : public GeneticEngineBase<BinaryIndividual, GAConfig>
{
protected: 
    void initialize() override;
    void evaluate() override;
    void selection() override;
    void crossover() override;
    void mutate() override;
    void save_history(std::vector<Genome>& target) override;

private:
    std::mt19937 rng{ std::random_device{}() };

    // Algorithm's intern methods
    uint32_t randGene();
    void decode_individual(BinaryIndividual&);
    
public:
    explicit GAEngine(const GAConfig& config) 
        : GeneticEngineBase<BinaryIndividual, GAConfig>(config) {}
};
