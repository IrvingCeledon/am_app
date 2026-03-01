#include "ga_engine.hpp"
#include <algorithm>

// ==========================================
// Intern helpers
// ==========================================

uint32_t GAEngine::randGene() {
    // Usamos 1ull para que el shift se haga en 64 bits de forma segura
    std::uniform_int_distribution<uint32_t> dist(
      0, static_cast<uint32_t>((1ull << configuration.bits) - 1)
    );
    
    return dist(rng);
}

// The engine is the one that knows how to translate bits to real numbers using Domains.
void GAEngine::decode_individual(BinaryIndividual& ind) 
{
    const size_t dims = configuration.domains.dimension();
    const double mask = static_cast<double>((1ull << configuration.bits) - 1);

    for (size_t i = 0; i < dims; ++i) {
        const auto& r = configuration.domains[i];
        ind.values()[i] = r.min + ind.genes()[i] * ((r.max - r.min) / mask);
    }
}

void GAEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : pop) {
        history_target.push_back(ind.values());
    }
}

bool GAEngine::check_stopping_criteria(size_t& patience, double& prev_best, double current_best)
{
    // 1. Target Fitness (We reached the mathematical goal)
    if (configuration.use_target && current_best <= configuration.target_fitness) {
        return true; 
    }

    // 2. Stagnation (We're stuck without improving)
    if (configuration.use_stagnation) 
    {
        // We only evaluate stagnation if it is not the first generation (Infinity)
        if (prev_best != std::numeric_limits<double>::max()) 
        {
            // Was the improvement leap less than epsilon? (We're stuck)
            if (std::abs(prev_best - current_best) < 1e-6) 
            {
                patience++; 
                if (patience >= configuration.stagnation_patience) {
                    return true; // Patience runs out
                }
                
                // We return false to keep prev_best with the last good real value.
                return false; 
            } 
            else 
            {
                // There was a significant improvement; we reset the patience counter.
                patience = 0; 
            }
        }
    }

    // We only update prev_best if there was no stagnation.
    prev_best = current_best;
    
    // Algorithm keeps running
    return false; 
}

// ==========================================
// Phases of the Genetic Algorithm
// ==========================================

void GAEngine::initialize() 
{
    const size_t dims = configuration.domains.dimension();
    
    for (auto& ind : pop) 
    {
        ind = BinaryIndividual(dims);
        
        for (size_t d = 0; d < dims; ++d) {
            ind.genes()[d] = randGene();
        }

        decode_individual(ind);
        ind.setCost(configuration.fitness(ind.values()));
    }
}

void GAEngine::evaluate() {
    for (auto& ind : pop) {
        decode_individual(ind);
        ind.setCost(configuration.fitness(ind.values()));
    }
}

void GAEngine::selection() {
    // Sorts from best to worst (lowest cost first) 
    std::sort(pop.begin(), pop.end()); 
    
    // We truncate the population (e.g., we keep the top 50%) aka elitism
    const size_t keep = static_cast<size_t>(pop.size() * configuration.selectionRate);
    pop.resize(keep);
}

void GAEngine::crossover() 
{
    const size_t target = configuration.populationSize;
    const size_t dims = configuration.domains.dimension();
    
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> parent_dist(0, pop.size() - 1);
    std::uniform_int_distribution<int> point_dist(1, configuration.bits - 1);
    
    while (pop.size() < target) 
    {
        const auto& p1 = pop[parent_dist(rng)];
        const auto& p2 = pop[parent_dist(rng)];
        
        BinaryIndividual child(dims);
        
        if (prob(rng) < configuration.crossoverRate) {
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

        pop.push_back(std::move(child));
    }
}

void GAEngine::mutate() 
{
    if (pop.size() <= 1) return;

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> bit_dist(0, configuration.bits - 1);

    const size_t dims = configuration.domains.dimension();

    // We start at 1 to apply elitism (not mutating the best individual)
    for (size_t i = 1; i < pop.size(); ++i) {
        for (size_t d = 0; d < dims; ++d) {
            if (prob(rng) < configuration.mutationRate) {
                pop[i].genes()[d] ^= (uint32_t(1) << bit_dist(rng));
            }
        }
    }
}

// ==========================================
// Orchestration (The Life Cycle)
// ==========================================

RunResult GAEngine::run() 
{
    RunResult result;

    initialize();
    std::sort(pop.begin(), pop.end());
    save_history(result.initialPopulation);
    
    // Variables for stagnation control
    size_t stagnation_counter = 0;
    double prev_best = std::numeric_limits<double>::max(); // We start in the worst possible scenario.

    for (size_t g = 0; g < configuration.generations; ++g) 
    {
        selection();
        crossover();
        mutate();
        evaluate();
        std::sort(pop.begin(), pop.end());

        double current_best = pop.best().getCost();
        result.bestFitnesses.push_back(current_best);

        if (g == configuration.generations / 2) {
            save_history(result.midPopulation);
        }

        // Verification of Early Stopping Criteria
        if ( check_stopping_criteria(stagnation_counter, prev_best, current_best) ) {
            break;
        }
    }

    save_history(result.finalPopulation);
    
    return result;
}
