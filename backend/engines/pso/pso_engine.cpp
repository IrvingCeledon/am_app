#include "pso_engine.hpp"

// ==========================================
// Intern helpers
// ==========================================

void PSOEngine::clamp_particle(Particle& p) {
    const size_t dims = this->configuration.domains.dimension();
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = this->configuration.domains[d];

        if (p.genes()[d] > r.max) {
            p.genes()[d] = r.max;
            p.getVelocity()[d] = 0.0;
        } else if (p.genes()[d] < r.min) {
            p.genes()[d] = r.min;
            p.getVelocity()[d] = 0.0; // Added missing semicolon
        }
    }
}

void PSOEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : this->pop) {
        history_target.push_back(ind.genes());
    }
}

// ==========================================
// PSO Phases
// ==========================================

void PSOEngine::initialize()
{
    const size_t dims = this->configuration.domains.dimension();

    // Initialize global best with worst possible scenario
    gbest = Particle(dims);
    gbest.setCost(std::numeric_limits<double>::max());

    for (auto& p : this->pop)
    {
        p = Particle(dims);

        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);

            p.genes()[d] = dist(rng);

            // Random initial velocity relative to the search space
            double range = r.max - r.min;
            std::uniform_real_distribution<double> v_dist(-range * 0.1, range * 0.1);
            p.getVelocity()[d] = v_dist(rng);
        }

        p.setCost(this->evaluate_fitness(p.genes()));

        // Initialize personal best
        p.getPBest() = p.genes();
        p.setPBestCost(p.getCost());

        // Update global best
        if (p.getCost() < gbest.getCost()) {
            gbest = p;
        }
    }
}

void PSOEngine::update_velocity() {
    const size_t dims = this->configuration.domains.dimension();
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (auto& p : this->pop) {
        for (size_t d = 0; d < dims; ++d) {
            double r1 = dist(rng);
            double r2 = dist(rng);

            double cognitive = this->configuration.c1 * r1 * (p.getPBest()[d] - p.genes()[d]);
            double social = this->configuration.c2 * r2 * (gbest.genes()[d] - p.genes()[d]);

            p.getVelocity()[d] = (this->configuration.w * p.getVelocity()[d]) + cognitive + social;
        }
    }
}

void PSOEngine::update_position() {
    const size_t dims = this->configuration.domains.dimension();

    for (auto& p : this->pop) {
        for (size_t d = 0; d < dims; ++d) {
            p.genes()[d] += p.getVelocity()[d];
        }
        // Clamp immediately after moving
        clamp_particle(p);
    }
}

void PSOEngine::evaluate_and_update_bests() {
    for (auto& p : this->pop) {
        p.setCost(this->evaluate_fitness(p.genes()));

        // Update personal best
        if (p.getCost() < p.getPBestCost()) {
            p.getPBest() = p.genes();
            p.setPBestCost(p.getCost());
        }

        // Update global best
        if (p.getCost() < gbest.getCost()) {
            gbest = p;
        }
    }
}

void PSOEngine::perform_iteration() {
    // Canonical execution order
    update_velocity();
    update_position();
    evaluate_and_update_bests();

    // EngineBase uses pop.best() to track current generation best.
    // We synchronize the best particle to index 0.
    auto it = std::min_element(this->pop.begin(), this->pop.end());
    if (it != this->pop.begin()) {
        std::swap(*this->pop.begin(), *it);
    }
}
