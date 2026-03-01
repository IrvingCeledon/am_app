#include "abc_engine.hpp"

// ==========================================
// Helpers internos
// ==========================================

void ABCEngine::clamp_individual(RealIndividual& ind) {
    const size_t dims = this->configuration.domains.dimension();
    for (size_t d = 0; d < dims; ++d) {
        const auto& r = this->configuration.domains[d];
        ind.genes()[d] = std::clamp(ind.genes()[d], r.min, r.max);
    }
}

double ABCEngine::calculate_abc_fitness(double cost) {
    // Para problemas de minimización, mapeamos el costo a un valor de fitness >= 0
    if (cost >= 0) return 1.0 / (cost + 1.0);
    return 1.0 + std::abs(cost);
}

void ABCEngine::save_history(std::vector<Genome>& history_target) {
    for (const auto& ind : this->pop) {
        history_target.push_back(ind.genes());
    }
}

// Genera una nueva solución vecina v_{ij} = x_{ij} + \phi (x_{ij} - x_{kj})
void ABCEngine::explore_neighborhood(size_t i) {
    RealIndividual mutant = this->pop[i];
    const size_t dims = this->configuration.domains.dimension();
    
    std::uniform_int_distribution<size_t> dim_dist(0, dims - 1);
    std::uniform_int_distribution<size_t> pop_dist(0, this->pop.size() - 1);
    std::uniform_real_distribution<double> phi_dist(-1.0, 1.0);

    size_t j = dim_dist(rng); // Dimensión aleatoria
    size_t k = i;
    while(k == i) { k = pop_dist(rng); } // Vecino distinto

    double phi = phi_dist(rng);
    mutant.genes()[j] = this->pop[i].genes()[j] + phi * (this->pop[i].genes()[j] - this->pop[k].genes()[j]);
    
    clamp_individual(mutant);
    mutant.setCost(this->configuration.fitness(mutant.genes()));
    
    // Selección codiciosa (Greedy Selection)
    if (mutant.getCost() < this->pop[i].getCost()) {
        this->pop[i] = mutant;
        this->fitness_abc[i] = calculate_abc_fitness(mutant.getCost());
        this->trials[i] = 0;
    } else {
        this->trials[i]++;
    }
}

// ==========================================
// Fases mapeadas al EngineBase
// ==========================================

void ABCEngine::initialize() {
    const size_t dims = this->configuration.domains.dimension();
    this->trials.assign(this->pop.size(), 0);
    this->prob.assign(this->pop.size(), 0.0);
    this->fitness_abc.assign(this->pop.size(), 0.0);

    for (size_t i = 0; i < this->pop.size(); ++i) {
        this->pop[i] = RealIndividual(dims);
        for (size_t d = 0; d < dims; ++d) {
            const auto& r = this->configuration.domains[d];
            std::uniform_real_distribution<double> dist(r.min, r.max);
            this->pop[i].genes()[d] = dist(rng);
        }
        this->pop[i].setCost(this->configuration.fitness(this->pop[i].genes()));
        this->fitness_abc[i] = calculate_abc_fitness(this->pop[i].getCost());
    }
}

// Calculamos las probabilidades para las abejas observadoras
void ABCEngine::selection() {
    double max_fitness = *std::max_element(fitness_abc.begin(), fitness_abc.end());
    for (size_t i = 0; i < this->pop.size(); ++i) {
        prob[i] = (0.9 * (fitness_abc[i] / max_fitness)) + 0.1;
    }
}

// Mapeamos las fases Empleadas y Observadoras al "Crossover"
void ABCEngine::crossover() {
    // 1. Fase Abejas Empleadas
    for (size_t i = 0; i < this->pop.size(); ++i) {
        explore_neighborhood(i);
    }

    // 2. Fase Abejas Observadoras
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    size_t t = 0;
    size_t i = 0;
    
    while (t < this->pop.size()) {
        if (dist(rng) < prob[i]) {
            explore_neighborhood(i);
            t++;
        }
        i = (i + 1) % this->pop.size();
    }
}

// Mapeamos la fase de Abejas Exploradoras (Scouts) a "Mutate"
void ABCEngine::mutate() {
    const size_t dims = this->configuration.domains.dimension();
    
    for (size_t i = 0; i < this->pop.size(); ++i) {
        if (this->trials[i] >= this->configuration.limit) {
            // Reiniciar fuente de comida (Abeja Exploradora)
            for (size_t d = 0; d < dims; ++d) {
                const auto& r = this->configuration.domains[d];
                std::uniform_real_distribution<double> dist(r.min, r.max);
                this->pop[i].genes()[d] = dist(rng);
            }
            this->pop[i].setCost(this->configuration.fitness(this->pop[i].genes()));
            this->fitness_abc[i] = calculate_abc_fitness(this->pop[i].getCost());
            this->trials[i] = 0;
        }
    }
}

// Vacío, porque la evaluación se hace en las fases correspondientes (Selección Codiciosa)
void ABCEngine::evaluate() {}
