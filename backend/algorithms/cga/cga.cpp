#include <cmath>        // For std::sqrt, std::cos, std::exp
#include <numbers>      // For std::numbers::pi and std::numbers::e (C++20)
#include "cga.hpp"

// The range of randGene depends on the bits of ga's instance.
double CGA::randGene(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    
    return dist(rng);
}
    
void CGA::initialize() 
{
  const auto& ranges = configuration.domains.ranges;
  
  for ( auto& individual : pop )
  {
    std::vector<Gene> genes;  // Genes for a new individual.
    genes.reserve( ranges.size() ); // Reserve the space based on the 'quantity' of ranges.
    
    // Each range can have different domains.
    for ( const auto& r : ranges ) {
      double value = randGene(r.min, r.max);  // Random value generation.
      genes.emplace_back(value, r.min, r.max);  // Contruct of object 'in-place'.
    }
    
    // Set the generated genes.
    individual.setChromosome(genes);
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
    
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<size_t> parent_dist(0, n_keep - 1);
    
    // This is arithmetic crossover.
    while ( pop.size() < target )
    {
      // Select parents randomly.
      const auto& p1 = pop[parent_dist(rng)];
      const auto& p2 = pop[parent_dist(rng)];
      
      Individual child; // New possible child.
      std::vector<Gene> child_genes;
      child_genes.reserve(p1.getChromosome().size());

      for (size_t i = 0; i < p1.getChromosome().size(); ++i) 
      {
          const auto& g1 = p1.getGene(i);
          const auto& g2 = p2.getGene(i);

          double value;
          if (prob(rng) < configuration.rates.crossover) {
              value = 0.5 * (g1.getValue() + g2.getValue()); // arithmetic mean
          } else {
              value = g1.getValue(); // copy parent
          }
          
          child_genes.emplace_back(value, g1.getMin(), g1.getMax());
      }                    
        
      child.setChromosome(child_genes);
      pop.push_back(child);
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
    for (const auto& ind : pop) {
        std::vector<double> values;
        
        for (const auto& g : ind.getChromosome()) {
            values.push_back(g.getValue());
        }
        
        pop_history.push_back(values);
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
