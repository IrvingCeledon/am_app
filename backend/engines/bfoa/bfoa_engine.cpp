#include "bfoa_engine.hpp"

// ==========================================
// Helpers
// ==========================================

double BFOAEngine::evaluate_fitness(const Genome& g) const {
    double val = configuration.fitness(g);
    return configuration.minimize ? val : -val;
}

void BFOAEngine::clamp_bacterium(Bacterium& b) {
    const size_t dims = this->configuration.domains.dimension();
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = this->configuration.domains[d];
        b.genes()[d] = std::clamp(b.genes()[d], r.min, r.max);
    }
}

void BFOAEngine::save_history(std::vector<Genome>& target) {
    for (const auto& ind : this->pop) {
        target.push_back(ind.genes());
    }
}

void BFOAEngine::push_log(RunResult& res, LogLevel lvl, const std::string& msg) {
    if (lvl >= configuration.verbosity) {
        res.logs.push_back({lvl, msg});
    }
}

bool BFOAEngine::check_stopping_criteria(RunResult& result, size_t& patience, double& prev_best, double current_best) {
    double internal_target = configuration.minimize ? configuration.target_fitness : -configuration.target_fitness;

    if (configuration.use_target && current_best <= internal_target) {
        double display_best = configuration.minimize ? current_best : -current_best;
        push_log(result, LogLevel::INFO_LVL, "Target fitness reached: " + std::to_string(display_best));
        return true;
    }

    if (!configuration.use_stagnation) {
        prev_best = current_best;
        return false;
    }

    if (prev_best == std::numeric_limits<double>::max()) {
        prev_best = current_best;
        return false;
    }

    const bool improved = (prev_best - current_best) >= 1e-6;

    if (improved) {
        patience = 0;
        prev_best = current_best;
        return false;
    }

    patience++;
    if (patience >= configuration.stagnation_patience) {
        push_log(result, LogLevel::WARNING_LVL,
                 "Early stopping: no improvement for " +
                 std::to_string(configuration.stagnation_patience) + " steps.");
        return true;
    }

    return false;
}

// ==========================================
// BFOA Core Phases
// ==========================================

void BFOAEngine::initialize() {
    const size_t dims = this->configuration.domains.dimension();

    global_best = Bacterium(dims);
    global_best.setCost(std::numeric_limits<double>::max());

    for (auto& b : this->pop) {
        b = Bacterium(dims);
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);
            b.genes()[d] = dist(rng);
        }
        b.setCost(evaluate_fitness(b.genes()));
        b.resetHealth();

        if (b.getCost() < global_best.getCost()) {
            global_best = b;
        }
    }
}

double BFOAEngine::calculate_swarming(size_t bacteria_index) {
    double j_cc = 0.0;
    const size_t dims = this->configuration.domains.dimension();
    const auto& current_bacterium = this->pop[bacteria_index];

    for (size_t i = 0; i < this->pop.size(); ++i) {
        if (i == bacteria_index) continue;

        double distance_sq = 0.0;
        for (size_t d = 0; d < dims; ++d) {
            double diff = current_bacterium.genes()[d] - this->pop[i].genes()[d];
            distance_sq += diff * diff;
        }

        double attract = -this->configuration.d_attract * std::exp(-this->configuration.w_attract * distance_sq);
        double repel = this->configuration.h_repel * std::exp(-this->configuration.w_repel * distance_sq);
        j_cc += (attract + repel);
    }
    return j_cc;
}

void BFOAEngine::chemotaxis() {
    const size_t dims = this->configuration.domains.dimension();
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (size_t i = 0; i < this->pop.size(); ++i) {
        auto& b = this->pop[i];

        double original_fitness = evaluate_fitness(b.genes()) + calculate_swarming(i);
        b.setCost(original_fitness);
        b.addHealth(original_fitness);

        // Generate random direction vector
        std::vector<double> delta(dims);
        double norm = 0.0;
        for (size_t d = 0; d < dims; ++d) {
            delta[d] = dist(rng);
            norm += delta[d] * delta[d];
        }
        norm = std::sqrt(norm);

        std::vector<double> direction(dims);
        for (size_t d = 0; d < dims; ++d) {
            direction[d] = delta[d] / (norm + 1e-10); // Prevent division by zero
        }

        // Tumble
        Bacterium perturbed = b;
        for (size_t d = 0; d < dims; ++d) {
            perturbed.genes()[d] += this->configuration.step_size * direction[d];
        }
        clamp_bacterium(perturbed);

        double perturbed_fitness = evaluate_fitness(perturbed.genes()) + calculate_swarming(i);

        // Swim
        size_t m = 0;
        while (m < this->configuration.run_length) {
            if (perturbed_fitness < original_fitness) {
                b = perturbed;
                original_fitness = perturbed_fitness;
                b.setCost(original_fitness);
                b.addHealth(original_fitness);

                for (size_t d = 0; d < dims; ++d) {
                    perturbed.genes()[d] += this->configuration.step_size * direction[d];
                }
                clamp_bacterium(perturbed);
                perturbed_fitness = evaluate_fitness(perturbed.genes()) + calculate_swarming(i);
                m++;
            } else {
                break;
            }
        }
    }
}

void BFOAEngine::reproduction() {
    // Sort population based on health (accumulated fitness)
    std::sort(this->pop.begin(), this->pop.end());

    size_t half_size = this->pop.size() / 2;

    // The healthier half replaces the other half
    for (size_t i = 0; i < half_size; ++i) {
        this->pop[i + half_size] = this->pop[i];
    }
}

void BFOAEngine::reorientation() {
    const size_t dims = this->configuration.domains.dimension();
    std::uniform_real_distribution<double> prob(0.0, 1.0);

    for (size_t i = 0; i < this->pop.size(); ++i) {
        if (prob(rng) < this->configuration.elimination_rate) {
            auto& b = this->pop[i];
            for (size_t d = 0; d < dims; ++d) {
                const auto& r = this->configuration.domains[d];
                std::uniform_real_distribution<double> dist(r.min, r.max);
                b.genes()[d] = dist(rng);
            }
            b.setCost(evaluate_fitness(b.genes()));
        }
    }
}

// ==========================================
// Main Runner
// ==========================================

RunResult BFOAEngine::run() {
    RunResult result;
    push_log(result, LogLevel::INFO_LVL, "Initializing BFOA population of size: " + std::to_string(configuration.populationSize));

    initialize();
    save_history(result.initialPopulation);

    size_t total_generations = configuration.elimination_dispersal_steps *
    configuration.reproduction_steps *
    configuration.chemotactic_steps;
    size_t current_gen = 0;

    size_t stagnation_counter = 0;
    double prev_best = std::numeric_limits<double>::max();
    bool early_stop = false;

    // The 3 canonical BFOA loops remain visible and clear
    for (size_t l = 0; l < configuration.elimination_dispersal_steps && !early_stop; ++l) {
        for (size_t k = 0; k < configuration.reproduction_steps && !early_stop; ++k) {

            for(auto& b : this->pop) { b.resetHealth(); }

            for (size_t j = 0; j < configuration.chemotactic_steps && !early_stop; ++j) {

                chemotaxis();

                // Find global best in current step
                for (const auto& b : this->pop) {
                    double true_cost = evaluate_fitness(b.genes());
                    if (true_cost < global_best.getCost()) {
                        global_best = b;
                        global_best.setCost(true_cost);
                    }
                }

                double current_best = global_best.getCost();
                double display_best = configuration.minimize ? current_best : -current_best;
                result.bestFitnesses.push_back(display_best);

                // Abstracted stopping logic
                early_stop = check_stopping_criteria(result, stagnation_counter, prev_best, current_best);

                current_gen++;
                if (current_gen == total_generations / 2 && !early_stop) {
                    save_history(result.midPopulation);
                }
            }
            if (!early_stop) reproduction();
        }
        if (!early_stop) reorientation();
    }

    push_log(result, LogLevel::INFO_LVL, "Execution completed. Best fitness: " + std::to_string(configuration.minimize ? global_best.getCost() : -global_best.getCost()));
    save_history(result.finalPopulation);

    return result;
}
