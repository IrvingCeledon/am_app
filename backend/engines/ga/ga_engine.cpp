#include "ga_engine.hpp"

// ==========================================
// Intern helpers
// ==========================================

uint32_t GAEngine::randGene() {
    // Usamos 1ull para que el shift se haga en 64 bits de forma segura
    std::uniform_int_distribution<uint32_t> dist(
      0, static_cast<uint32_t>((1ull << this->configuration.bits) - 1)
    );
    
    return dist(rng);
}

// The engine is the one that knows how to translate bits to real numbers using Domains.
void GAEngine::decode_individual(BinaryIndividual& ind) 
{
    const size_t dims = this->configuration.domains.dimension();
    const double mask = static_cast<double>((1ull << this->configuration.bits) - 1);

    for (size_t i = 0; i < dims; ++i) {
        const auto& r = this->configuration.domains[i];
        ind.values()[i] = r.min + ind.genes()[i] * ((r.max - r.min) / mask);
    }
}

void GAEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : this->pop) {
        history_target.push_back(ind.values());
    }
}

// ==========================================
// Phases of the Genetic Algorithm
// ==========================================

void GAEngine::initialize() 
{
    const size_t dims = this->configuration.domains.dimension();
    
    for (auto& ind : this->pop) 
    {
        ind = BinaryIndividual(dims);
        
        for (size_t d = 0; d < dims; ++d) {
            ind.genes()[d] = randGene();
        }

        decode_individual(ind);
        ind.setCost(this->configuration.fitness(ind.values()));
    }
}

void GAEngine::evaluate() {
    for (auto& ind : this->pop) {
        decode_individual(ind);
        ind.setCost(this->configuration.fitness(ind.values()));
    }
}

void GAEngine::selection() {
    // Sorts from best to worst (lowest cost first) 
    std::sort(this->pop.begin(), this->pop.end()); 
    
    // We truncate the population (e.g., we keep the top 50%) aka elitism
    const size_t keep = static_cast<size_t>(this->pop.size() * this->configuration.selectionRate);
    this->pop.resize(keep);
}

void GAEngine::crossover() 
{
    const size_t target = this->configuration.populationSize;
    const size_t dims = this->configuration.domains.dimension();
    
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> parent_dist(0, this->pop.size() - 1);
    std::uniform_int_distribution<int> point_dist(1, this->configuration.bits - 1);
    
    while (this->pop.size() < target) 
    {
        const auto& p1 = this->pop[parent_dist(rng)];
        const auto& p2 = this->pop[parent_dist(rng)];
        
        BinaryIndividual child(dims);
        
        if (prob(rng) < this->configuration.crossoverRate) {
            int point = point_dist(rng);
            uint32_t mask_low = static_cast<uint32_t>((1ull << point) - 1);
            uint32_t mask_high = ~mask_low;

            for (size_t d = 0; d < dims; ++d) {
                child.genes()[d] = (p1.genes()[d] & mask_high) | 
                                   (p2.genes()[d] & mask_low);
            }
        } else {
            child = p1; // If there is no crossbreeding, the offspring is a clone of the parent 1
        }

        this->pop.push_back(std::move(child));
    }
}

void GAEngine::mutate() 
{
    if (this->pop.size() <= 1) return;

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> bit_dist(0, this->configuration.bits - 1);

    const size_t dims = this->configuration.domains.dimension();

    // We start at 1 to apply elitism (not mutating the best individual)
    for (size_t i = 1; i < this->pop.size(); ++i) {
        for (size_t d = 0; d < dims; ++d) {
            if (prob(rng) < this->configuration.mutationRate) {
                this->pop[i].genes()[d] ^= (uint32_t(1) << bit_dist(rng));
            }
        }
    }
}
