#pragma once

#include <random>
#include "../../evo_core/real_individual.hpp"
#include "../engine_base.hpp"
#include "abc_config.hpp"

class ABCEngine : public EngineBase<RealIndividual, ABCConfig>
{
protected:
    void initialize() override;
    void save_history(std::vector<Genome>& target) override;
    
    void perform_iteration() override
    {
        RealIndividual best_so_far = this->pop.best();
        
        send_employed_bees();
        calculate_probabilities();
        send_onlooker_bees();
        send_scout_bees();
        
        // Artificial elitism.
        if (best_so_far < this->pop.best())
        {
            auto worst_it = std::max_element(this->pop.begin(), this->pop.end());
            size_t worst_idx = std::distance(this->pop.begin(), worst_it);
        
            *worst_it = best_so_far;
            this->mapped_fitness[worst_idx] = calculate_abc_fitness(best_so_far.getCost());
            this->trial_counters[worst_idx] = 0;
        }
    }

private:
    std::mt19937 rng{ std::random_device{}() };
    
    void send_employed_bees();
    void calculate_probabilities();
    void send_onlooker_bees();
    void send_scout_bees();
    
    std::vector<size_t> trial_counters;
    std::vector<double> selection_probabilities;
    std::vector<double> mapped_fitness;
    
    // Helpers
    void clamp_individual(RealIndividual& ind);
    double calculate_abc_fitness(double cost);
    void explore_neighborhood(size_t i);

public: 
    explicit ABCEngine(const ABCConfig& config) 
        : EngineBase<RealIndividual, ABCConfig>(config) {}
};
