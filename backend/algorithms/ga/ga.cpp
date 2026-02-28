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
  for ( auto& individual : pop )
  {
    auto& chromo = individual.getChromosome();
    
    // Generete random chromosomes
    chromo.setX( randGene() );
    chromo.setY( randGene() );
        
    // Decode
    individual.decode(
      configuration.domains.x.min, 
      configuration.domains.x.max,
      configuration.domains.y.min, 
      configuration.domains.y.max,
      configuration.bits
    );
        
    // Evaluate -> This is done here because is more efficent than call evaluate, and starts another loop, inmediatly after initialization.
    individual.evaluate(configuration.fitness);
  }
}

void GA::evaluate()
{
    const auto& dom = configuration.domains;

    for (auto& ind : pop)
    {
        // Update decode values. . .
        ind.decode(dom.x.min, dom.x.max,
                   dom.y.min, dom.y.max,
                   configuration.bits);

        // and evaluate.
        ind.evaluate(configuration.fitness);
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
    
    while ( pop.size() < target )
    {
      const auto& p1 = pop[parent_dist(rng)];
      const auto& p2 = pop[parent_dist(rng)];
      
      Individual child;
      
      if ( prob(rng) < configuration.rates.crossover )
      {
        int point = point_dist(rng);
        
        uint16_t mask_low = (1u << point) - 1;
        uint16_t mask_high = ~mask_low;
        
        uint16_t x = 
          (p1.getChromosome().getX() & mask_high) |
          (p2.getChromosome().getX() & mask_low);

        uint16_t y =
          (p1.getChromosome().getY() & mask_high) |
          (p2.getChromosome().getY() & mask_low);
          
        child.getChromosome().setX(x);
        child.getChromosome().setY(y);
      } 
      else  // if crossover fails, copy parent.
      {
        child = p1;
      }

      child.decode(configuration.domains.x.min,
                   configuration.domains.x.max,
                   configuration.domains.y.min,
                   configuration.domains.y.max,
                   configuration.bits);

      child.evaluate(configuration.fitness);

      pop.push_back(child);
    }
}

void GA::mutate()
{
  if (pop.size() <= 1) return;
  
  for (size_t i = 1; i < pop.size(); i++) {
    pop[i].getChromosome().mutate( rng, configuration.rates.mutation, configuration.bits );
  }
}

// f(x) = x³ + 4x² - 4x + 1, x_range: [-5, 3]
double cost_function_a(double x, double y) {
    (void)y;
    return ( x*x*x + 4*x*x - 4*x + 1 );
}

// f(x) = x⁴ + 5x³ + 4x² - 4x + 1, x_range: {-5, 3]
double cost_function_b(double x, double y) {
    (void)y;
    return ( x*x*x*x + 5*x*x*x + 4*x*x - 4*x + 1 );
}

// f(x, y) = e - 20exp {-20sqrt[(x² + y²)/2] - exp {[cos(2pix) + cos(2piy)]/2}, x/y_range: [-5, 5]
double cost_function_c(double x, double y)
{
    const double e  = std::numbers::e;
    const double pi = std::numbers::pi;

    double term1 = -20.0 * std::exp( -20.0 * std::sqrt((x*x + y*y) / 2.0) );

    double term2 = -std::exp( (std::cos(2*pi*x) + std::cos(2*pi*y)) / 2.0 );

    return e + term1 + term2;
}

RunResult GA::run()
{
    RunResult result;

    // First pop:
    initialize(); // I don't evaluate here, because i do it in initialize.
    sort();

    // Initial pop:
    for (const auto& ind : pop)
        result.initialPopulation.emplace_back(
            ind.getDecodedX(),
            ind.getDecodedY()
        );

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
        if (g == configuration.generations / 2)
        {
            for (const auto& ind : pop)
                result.midPopulation.emplace_back(
                    ind.getDecodedX(),
                    ind.getDecodedY()
                );
        }
    }

    // Final pop:
    for (const auto& ind : pop)
        result.finalPopulation.emplace_back(
            ind.getDecodedX(),
            ind.getDecodedY()
        );

    return result;
}
