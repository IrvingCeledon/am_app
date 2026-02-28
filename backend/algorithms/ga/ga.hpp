#pragma once
#include <random>
#include "population.hpp"
#include "ga_config.hpp"

class GA
{
  private:
    std::mt19937 rng{ std::random_device{}() }; // Initialization of random motor.
    GAConfig configuration;
    Population pop;
    
    uint16_t randGene();
    
    // GA phases:
    void initialize();
    void evaluate();
    void sort() noexcept;
    void select();
    void crossover();
    void mutate();
    
  public: 
    ~GA() = default;
    GA() = default;
    explicit GA(const GAConfig& config) 
      : configuration(config),
        pop(config.populationSize)
      {}
    
    // Read-only getter:
    const Population& population() const noexcept { return pop; }
    
    // Helpers:
    const Individual& best() const noexcept { return pop.best(); }
    size_t size() const noexcept { return pop.size(); }
    
    auto begin() noexcept { return pop.begin(); }
    auto end() noexcept { return pop.end(); }
    
    auto begin() const noexcept { return pop.begin(); }
    auto end() const noexcept { return pop.end(); }
    
    // update_config(config) -> on hold
    RunResult run();
    void update_config(const GAConfig& config) { configuration = config; }
};

// Global declaration of cost functions
double cost_function_a(double, double);
double cost_function_b(double, double);
double cost_function_c(double, double);
