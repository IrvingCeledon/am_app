#pragma once
#include <random>
#include <vector>
#include "individual.hpp"
#include "../evo_core/population.hpp"
#include "cga_config.hpp"

class CGA
{
  private:
    std::mt19937 rng{ std::random_device{}() }; // Initialization of random motor.
    CGAConfig configuration;
    Population<Individual> pop;
    
    // CGA phases:
    void initialize();
    void evaluate();
    void sort() noexcept;
    void select();
    void crossover();
    void mutate();
    
    void save_pop(std::vector<std::vector<double>>&);
    
  public: 
    ~CGA() = default;
    CGA() = default;
    explicit CGA(const CGAConfig& config) 
      : configuration(config), pop(config.populationSize)
      {}
    
    // Read-only getter:
    const Population<Individual>& population() const noexcept { return pop; }
    
    // Helpers:
    const Individual& best() const noexcept { return pop.best(); }
    size_t size() const noexcept { return pop.size(); }
    
    auto begin() noexcept { return pop.begin(); }
    auto end() noexcept { return pop.end(); }
    
    auto begin() const noexcept { return pop.begin(); }
    auto end() const noexcept { return pop.end(); }
    
    // update_config(config) -> on hold
    RunResult run();
};

// Global declaration of cost functions
double ackley_function(const std::vector<double>&);
double sphere_function(const std::vector<double>&);
