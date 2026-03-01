#include <cmath>        // For std::sqrt, std::cos, std::exp
#include <numbers>      // For std::numbers::pi and std::numbers::e (C++20)
#include "cga.hpp"
    
void CGA::initialize() 
{
    for (auto& individual : pop)
    {
        individual.randomize(configuration.domains, rng);
        individual.evaluate(configuration.fitness);
    }
}

void CGA::evaluate()
{
    for (auto& ind :  pop) {
      ind.evaluate(configuration.fitness);
    }
}

// This method benefits from the glorified container.
void CGA::sort() noexcept { 
  std::sort( pop.begin(), pop.end() ); 
}

// Reduces the population basen on the selection rate.
void CGA::select() {
  const size_t n_keep = static_cast<size_t>(pop.size() * configuration.rates.selection);
  pop.resize(n_keep);
}

void CGA::crossover()
{
    const size_t target = configuration.populationSize;
    const size_t n_keep   = pop.size();
    
    std::uniform_int_distribution<size_t> parent_dist(0, n_keep - 1);
    
    // This is arithmetic crossover.
    while (pop.size() < target)
    {
        const auto& p1 = pop[parent_dist(rng)];
        const auto& p2 = pop[parent_dist(rng)];

        pop.push_back(
            p1.crossoverWith(p2, rng, configuration.rates.crossover));
    }
}

void CGA::mutate()
{
    for (auto& ind : pop) {
        ind.mutate(rng, configuration.rates.mutation, configuration.mutation_scale);
    }
}

// f(x, y) = e - 20 *exp(-0.2 * sqrt((x^2 + y^2)/2)) - exp((cos(2*pi*x) + cos(2*pi*y))/2)
// Range: [-5, 5]
double ackley_function(const std::vector<double>& genes) 
{
    double x = genes.at(0);
    double y = genes.at(1);

    const double e  = std::numbers::e;
    const double pi = std::numbers::pi;

    double term1 = -20.0 * std::exp( -.2 * std::sqrt((x*x + y*y) / 2.0) );

    double term2 = -std::exp( (std::cos(2*pi*x) + std::cos(2*pi*y)) / 2.0 );

    return e + term1 + term2;
}

// Sphere: f(x) = sum(x[i]^2) for i in 1..n
// Range: [-5.12, 5.12], n = 4 && n = 6
double sphere_function(const std::vector<double>& genes)
{
    double sum = 0.0;
    
    for (double x : genes) {
        sum += x * x;
    }
    
    return sum;
}


void CGA::save_pop(std::vector<std::vector<double>>& pop_history)
{
    for (const auto& ind : pop) 
    {
        std::vector<double> values;
        values.reserve(ind.getChromosome().size());  

        for (const auto& g : ind.getChromosome()) {
            values.push_back(g.getValue());
        }

        pop_history.push_back(std::move(values));
    }
}

RunResult CGA::run()
{
    RunResult result;

    // First pop:
    initialize(); // I don't evaluate here, because i do it in initialize.
    sort();

    // Saves Initial pop:
    save_pop(result.initialPopulation);

    // Generational loop
    for (size_t g = 0; g < configuration.generations; ++g)
    {
        select();
        crossover();
        mutate();
        evaluate();
        sort();

        // Saves fitness after evaluation:
        result.bestFitnesses.push_back(pop[0].getCost());

        // Saves mid pop:
        if (g == configuration.generations / 2) {
            save_pop(result.midPopulation);
        }
    }

    // Saves Final pop:
    save_pop(result.finalPopulation);

    return result;
}
