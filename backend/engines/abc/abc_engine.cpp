#include "abc_engine.hpp"

// ==========================================
// Intern helpers
// ==========================================

void ABCEngine::clamp_individual(RealIndividual& ind) {
    const size_t dims = this->configuration.domains.dimension();
    
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = this->configuration.domains[d];
        ind.genes()[d] = std::clamp(ind.genes()[d], r.min, r.max);
    }
}

double ABCEngine::calculate_abc_fitness(double cost) {
    if (cost >= 0) return 1.0 / (cost + 1.0);
    return 1.0 + std::abs(cost);
}

void ABCEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : this->pop) {
        history_target.push_back(ind.genes());
    }
}

// Genereta a new neighborhood pop, v_{ij} = x_{ij} + \phi (x_{ij} - x_{kj}).
void ABCEngine::explore_neighborhood(size_t i) 
{
    auto& ind = this->pop[i];
    RealIndividual mutant = ind;
    const size_t dims = this->configuration.domains.dimension();
    
    std::uniform_int_distribution<size_t> dim_dist(0, dims - 1);
    std::uniform_int_distribution<size_t> pop_dist(0, this->pop.size() - 1);
    std::uniform_real_distribution<double> phi_dist(-1.0, 1.0);

    size_t j = dim_dist(rng); 
    size_t k = i;
    while(k == i) { k = pop_dist(rng); } 

    double phi = phi_dist(rng);
    double v_ij = ind.genes()[j] + phi * (ind.genes()[j] - this->pop[k].genes()[j]); 
    
    if (this->configuration.use_gbest) {
        std::uniform_real_distribution<double> psi_dist(0.0, 1.5);
        double psi = psi_dist(rng);
        const auto& best_ind = this->pop.best();
        
        v_ij += psi * (best_ind.genes()[j] - ind.genes()[j]);
    }
                  
    mutant.genes()[j] = v_ij;
    
    clamp_individual(mutant);
    mutant.setCost(this->evaluate_fitness(mutant.genes()));
    
    // Greedy Selection
    if (mutant.getCost() < ind.getCost()) {
        ind = mutant;
        this->mapped_fitness[i] = calculate_abc_fitness(mutant.getCost());
        this->trial_counters[i] = 0;
    } else {
        this->trial_counters[i]++;
    }
}

// ==========================================
// ABC phases
// ==========================================

void ABCEngine::initialize() 
{
    const size_t dims = this->configuration.domains.dimension();
    this->trial_counters.assign(this->pop.size(), 0);
    this->selection_probabilities.assign(this->pop.size(), 0.0);
    this->mapped_fitness.assign(this->pop.size(), 0.0);

    for (size_t i = 0; i < this->pop.size(); ++i) 
    {
        auto& ind = this->pop[i];
        ind = RealIndividual(dims);
        
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            // Initial food sources sites:
            std::uniform_real_distribution<double> dist(r.min, r.max);
            ind.genes()[d] = dist(rng);
        }
        
        ind.setCost(this->evaluate_fitness(ind.genes()));
        // Compute fitness values:
        this->mapped_fitness[i] = calculate_abc_fitness(ind.getCost());
    }
}

void ABCEngine::send_employed_bees()
{
    for (size_t i = 0; i < this->pop.size(); ++i) {
        explore_neighborhood(i);
    }
}

void ABCEngine::calculate_probabilities()
{
    double max_fitness = *std::max_element(mapped_fitness.begin(), mapped_fitness.end());
    
    for (size_t i = 0; i < this->pop.size(); ++i) {
        selection_probabilities[i] = (0.9 * (mapped_fitness[i] / max_fitness)) + 0.1;
    }
}

void ABCEngine::send_onlooker_bees()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    size_t i = 0;
    size_t j = 0;
    
    while (j < this->pop.size()) 
    {
        if (dist(rng) < selection_probabilities[i]) {
            explore_neighborhood(i);
            j++;
        }
        
        i = (i + 1) % this->pop.size();
    }
}
    
void ABCEngine::send_scout_bees()
{
    auto max_it = std::max_element(this->trial_counters.begin(), this->trial_counters.end());
    size_t max_index = std::distance(this->trial_counters.begin(), max_it);

    if (this->trial_counters[max_index] >= this->configuration.limit) 
    {
        const size_t dims = this->configuration.domains.dimension();
        
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);
            this->pop[max_index].genes()[d] = dist(rng);
        }
        
        this->pop[max_index].setCost(this->evaluate_fitness(this->pop[max_index].genes()));
        this->mapped_fitness[max_index] = calculate_abc_fitness(this->pop[max_index].getCost());
        this->trial_counters[max_index] = 0;
    }
}
