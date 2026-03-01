#include "cga_engine.hpp"
#include <algorithm>

// ==========================================
// Intern helpers
// ==========================================

void CGAEngine::clamp_individual(RealIndividual& ind) {
    const size_t dims = configuration.domains.dimension();
    
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = configuration.domains[d];
        ind.genes()[d] = std::clamp(ind.genes()[d], r.min, r.max);
    }
}

void CGAEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : pop) {
        // In CGA, the genotype (genes) is identical to the phenotype (genome)
        history_target.push_back(ind.genes());
    }
}

bool CGAEngine::check_stopping_criteria(size_t& patience, double& prev_best, double current_best)
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
// Phases of the Continuous Genetic Algorithm
// ==========================================

void CGAEngine::initialize() 
{
    const size_t dims = configuration.domains.dimension();
    
    for (auto& ind : pop) 
    {
        ind = RealIndividual(dims);
        
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);
            ind.genes()[d] = dist(rng);
        }

        ind.setCost(configuration.fitness(ind.genes()));
    }
}

void CGAEngine::evaluate() {
    for (auto& ind : pop) {
        ind.setCost(configuration.fitness(ind.genes()));
    }
}

void CGAEngine::selection() {
    // Sorts from best to worst (lowest cost first) 
    std::sort(pop.begin(), pop.end()); 
    
    // We truncate the population (e.g., we keep the top 50%) aka elitism
    const size_t keep = static_cast<size_t>(pop.size() * configuration.selectionRate);
    pop.resize(keep);
}

void CGAEngine::crossover() 
{
    const size_t target = configuration.populationSize;
    const size_t dims = configuration.domains.dimension();
    
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> parent_dist(0, pop.size() - 1);
    
    while (pop.size() < target) 
    {
        const auto& p1 = pop[parent_dist(rng)];
        const auto& p2 = pop[parent_dist(rng)];
        
        RealIndividual child(dims);
        
        // Arithmetic cross (parents' mean)
        for (size_t d = 0; d < dims; ++d) 
        {
            if (prob(rng) < configuration.crossoverRate) {
                child.genes()[d] = 0.5 * (p1.genes()[d] + p2.genes()[d]);
            } else {
                child.genes()[d] = p1.genes()[d];
            }
        }

        pop.push_back(std::move(child));
    }
}

void CGAEngine::mutate() 
{
    if (pop.size() <= 1) return;

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::normal_distribution<double> noise(0.0, configuration.mutationScale);

    const size_t dims = configuration.domains.dimension();

    // We start at 1 to apply elitism (not mutating the best individual)
    for (size_t i = 1; i < pop.size(); ++i) 
    {
        bool mutated = false;
        
        for (size_t d = 0; d < dims; ++d) {
            if (prob(rng) < configuration.mutationRate) {
                pop[i].genes()[d] += noise(rng);
                mutated = true;
            }
        }
        
        // If the mutation pushed values ​​out of bounds, we corrected them.
        if (mutated) {
            clamp_individual(pop[i]);
        }
    }
}

// ==========================================
// Orchestration (The Life Cycle)
// ==========================================

RunResult CGAEngine::run() 
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
