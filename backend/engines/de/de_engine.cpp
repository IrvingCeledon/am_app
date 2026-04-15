#include "de_engine.hpp"

void DEEngine::clamp_individual(RealIndividual& ind) {
    const size_t dims = this->configuration.domains.dimension();
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = this->configuration.domains[d];
        ind.genes()[d] = std::clamp(ind.genes()[d], r.min, r.max);
    }
}

void DEEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : this->pop) {
        history_target.push_back(ind.genes());
    }
}

void DEEngine::initialize() {
    const size_t dims = this->configuration.domains.dimension();
    for (auto& ind : this->pop) {
        ind = RealIndividual(dims);
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);
            ind.genes()[d] = dist(rng);
        }
        ind.setCost(this->evaluate_fitness(ind.genes()));
    }
    std::sort(this->pop.begin(), this->pop.end());
}

// ==========================================
// DE Phases
// ==========================================

Genome DEEngine::mutate(size_t target_idx) {
    const size_t pop_size = this->pop.size();
    const size_t dims = this->configuration.domains.dimension();

    size_t r1, r2, r3;
    std::uniform_int_distribution<size_t> idx_dist(0, pop_size - 1);

    do { r1 = idx_dist(rng); } while (r1 == target_idx);
    do { r2 = idx_dist(rng); } while (r2 == target_idx || r2 == r1);
    do { r3 = idx_dist(rng); } while (r3 == target_idx || r3 == r1 || r3 == r2);

    Genome mutant(dims);
    for (size_t j = 0; j < dims; ++j) {
        // v_i = x_r1 + F * (x_r2 - x_r3)
        mutant[j] = this->pop[r1].genes()[j] +
        this->configuration.F * (this->pop[r2].genes()[j] - this->pop[r3].genes()[j]);
    }
    return mutant;
}

Genome DEEngine::recombine(const Genome& target, const Genome& mutant) {
    const size_t dims = this->configuration.domains.dimension();
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> dim_dist(0, dims - 1);

    size_t R = dim_dist(rng); // Ensure at least one parameter mutates
    Genome trial_genes(dims);

    for (size_t j = 0; j < dims; ++j) {
        if (prob(rng) <= this->configuration.CR || j == R) {
            trial_genes[j] = mutant[j];
        } else {
            trial_genes[j] = target[j];
        }
    }
    return trial_genes;
}

void DEEngine::select(Population<RealIndividual>& next_gen, size_t target_idx, RealIndividual& trial) {
    clamp_individual(trial);
    trial.setCost(this->evaluate_fitness(trial.genes()));

    // Greedy selection
    if (trial.getCost() <= this->pop[target_idx].getCost()) {
        next_gen[target_idx] = trial;
    } else {
        next_gen[target_idx] = this->pop[target_idx];
    }
}

void DEEngine::perform_iteration() {
    const size_t pop_size = this->pop.size();
    Population<RealIndividual> next_gen(pop_size);

    for (size_t i = 0; i < pop_size; ++i) {
        // Phase 1: Mutation
        Genome mutant = mutate(i);

        // Phase 2: Crossover (Recombination)
        RealIndividual trial(this->configuration.domains.dimension());
        trial.genes() = recombine(this->pop[i].genes(), mutant);

        // Phase 3: Selection
        select(next_gen, i, trial);
    }

    // Update and sort population
    for(size_t i = 0; i < pop_size; ++i) {
        this->pop[i] = next_gen[i];
    }
    std::sort(this->pop.begin(), this->pop.end());
}
