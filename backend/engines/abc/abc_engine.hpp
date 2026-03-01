#pragma once

#include <random>
#include <vector>
#include "../../evo_core/real_individual.hpp"
#include "../engine_base.hpp"
#include "abc_config.hpp"

class ABCEngine : public EngineBase<RealIndividual, ABCConfig>
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
    
    // Arrays paralelos a la población para uso específico de ABC
    std::vector<size_t> trials;       // Contador de estancamiento para cada fuente
    std::vector<double> prob;         // Probabilidad de selección para observadoras
    std::vector<double> fitness_abc;  // Fitness mapeado (ABC maximiza fitness aunque minimice costo)

    void clamp_individual(RealIndividual& ind);
    double calculate_abc_fitness(double cost);
    void explore_neighborhood(size_t i); // Lógica compartida por empleadas y observadoras

public: 
    explicit ABCEngine(const ABCConfig& config) 
        : EngineBase<RealIndividual, ABCConfig>(config) {}
};
