#include <cmath>        // For std::sqrt, std::cos, std::exp
#include <numbers>      // For std::numbers::pi and std::numbers::e (C++20)
#include "ga.hpp"

// The range of randGene depends on the bits of ga's instance.
uint16_t GA::randGene() {
    std::uniform_int_distribution<uint16_t> dist(
        0, (1u << configuration.bits) - 1
    );
    
    return dist(rng);
}
 
void GA::initialize() 
{
    const size_t dims = configuration.domains.dimension();
    
    for ( auto& individual : pop )
    {
        individual = Individual(dims);
        
        auto& genes = individual.genes();
      
        for ( size_t d = 0; d < dims; ++d) {
            genes[d] = randGene();
        }

        individual.decode(configurarion.domains, configurarion.bits);
        individual.evaluate(configuration.fitness);
    }
}

void GA::evaluate()
{
    for (auto& individual : pop) {
        individual.decode(configurarion.domains, configuration.bits);
        individual.evaluate(configuration.fitness);
    }
}

// This method benefits from the glorified container.
void GA::sort() noexcept { 
  std::sort( pop.begin(), pop.end() ); 
}

// Reduces the population basen on the selection rate.
void GA::select() {
  const size_t n_keep = static_cast<size_t>(pop.size() * configuration.rates.selection);
  
  pop.resize(n_keep);
}

void GA::crossover()
{
    const size_t target = configuration.populationSize;
    const size_t n_keep   = pop.size();
    
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> parent_dist(0, n_keep - 1);
    std::uniform_int_distribution<int> point_dist(1, configuration.bits - 1);
    
    const size_t dims = configuration.domains.dimension();
    
    // This is single point crossover.
    while ( pop.size() < target )
    {
        // Select parents randomly.
        const auto& p1 = pop[parent_dist(rng)];
        const auto& p2 = pop[parent_dist(rng)];
        
        Individual child(dims); // New possible child.
        
        if ( prob(rng) < configuration.rates.crossover )
        {
          // Selects a random cut point.
          int point = point_dist(rng);
          
          // This emulate the cut procces with left shift operator (this cut in the exact pint) - 1 for p1.
          // And the cut point is made it with the negation of mask_low.
          // Example with: bit = 8, point = 3
          uint16_t mask_low = (1u << point) - 1;  // 1 << 3 = 00001000 - 1 = 00000111
          uint16_t mask_high = ~mask_low; // ~mask_low = 11111000

          for ( size_t d = 0; d < dims; ++d ) 
          {
              uint16_t g = 
                  (p1.genes()[d] & mask_high) |
                  (p2.genes()[d] & mask_low);
                  
              child.genes()[d] = g;
          }
        } 
        else  // if crossover fails, copy parent.
        {
          child = p1;
        }

        pop.push_back(child);
    }
}

void GA::mutate()
{
    if (pop.size() <= 1) return;
    // Calls the mutation method for each individual, with the current rng and bits.
    for (size_t i = 1; i < pop.size(); i++) {
        pop[i].genes().mutate( rng, configuration.rates.mutation, configuration.bits );
    }
}

// f(x) = x³ + 4x² - 4x + 1, x_range: [-5, 3]
double cost_function_a(const Genome& g) {
    (void)y;
    return ( x*x*x + 4*x*x - 4*x + 1 );
}

// f(x) = x⁴ + 5x³ + 4x² - 4x + 1, x_range: {-5, 3]
double cost_function_b(const Genome& g) {
    (void)y;
    return ( x*x*x*x + 5*x*x*x + 4*x*x - 4*x + 1 );
}

// f(x, y) = e - 20exp {-20sqrt[(x² + y²)/2] - exp {[cos(2pix) + cos(2piy)]/2}, x/y_range: [-5, 5]
double cost_function_c(const Genome& g)
{
    const double e  = std::numbers::e;
    const double pi = std::numbers::pi;

    double term1 = -20.0 * std::exp( -20.0 * std::sqrt((x*x + y*y) / 2.0) );

    double term2 = -std::exp( (std::cos(2*pi*x) + std::cos(2*pi*y)) / 2.0 );

    return e + term1 + term2;
}

void GA::save_pop(std::vector<Genome>>& history)
{
    for (const auto& individual : pop) {
        pop_history.push_back(individual.values());
    }
}

RunResult GA::run()
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
