#include "cga_engine.hpp"

// ==========================================
// Intern helpers
// ==========================================

void CGAEngine::clamp_individual(RealIndividual& ind) {
    const size_t dims = this->configuration.domains.dimension();
    
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = this->configuration.domains[d];
        ind.genes()[d] = std::clamp(ind.genes()[d], r.min, r.max);
    }
}

void CGAEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : this->pop) {
        // In CGA, the genotype (genes) is identical to the phenotype (genome)
        history_target.push_back(ind.genes());
    }
}

// ==========================================
// Phases of the Continuous Genetic Algorithm
// ==========================================

void CGAEngine::initialize() 
{
    const size_t dims = this->configuration.domains.dimension();
    
    for (auto& ind : this->pop) 
    {
        ind = RealIndividual(dims);
        
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);
            ind.genes()[d] = dist(rng);
        }

        ind.setCost(this->configuration.fitness(ind.genes()));
    }
}

void CGAEngine::evaluate() {
    for (auto& ind : this->pop) {
        ind.setCost(this->configuration.fitness(ind.genes()));
    }
}

void CGAEngine::selection() {
    std::sort(this->pop.begin(), this->pop.end()); 
    
    // We truncate the population (e.g., we keep the top 50%) aka elitism
    const size_t keep = static_cast<size_t>(this->pop.size() * this->configuration.selectionRate);
    this->pop.resize(keep);
}

void CGAEngine::crossover() 
{
    const size_t target = this->configuration.populationSize;
    const size_t dims = this->configuration.domains.dimension();
    
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> parent_dist(0, this->pop.size() - 1);
    
    while (this->pop.size() < target) 
    {
        const auto& p1 = this->pop[parent_dist(rng)];
        const auto& p2 = this->pop[parent_dist(rng)];
        
        RealIndividual child(dims);
        
        // Arithmetic cross (parents' mean)
        for (size_t d = 0; d < dims; ++d) 
        {
            if (prob(rng) < this->configuration.crossoverRate) {
                child.genes()[d] = 0.5 * (p1.genes()[d] + p2.genes()[d]);
            } else {
                child.genes()[d] = p1.genes()[d];
            }
        }

        this->pop.push_back(std::move(child));
    }
}

void CGAEngine::mutate() 
{
    if (this->pop.size() <= 1) return;

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::normal_distribution<double> noise(0.0, this->configuration.mutationScale);

    const size_t dims = this->configuration.domains.dimension();

    // We start at 1 to apply elitism (not mutating the best individual)
    for (size_t i = 1; i < this->pop.size(); ++i) 
    {
        bool mutated = false;
        
        for (size_t d = 0; d < dims; ++d) {
            if (prob(rng) < this->configuration.mutationRate) {
                this->pop[i].genes()[d] += noise(rng);
                mutated = true;
            }
        }
        
        // If the mutation pushed values ​​out of ranges, we corrected them.
        if (mutated) {
            clamp_individual(this->pop[i]);
        }
    }
}
