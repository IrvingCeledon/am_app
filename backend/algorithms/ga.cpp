#include "ga.hpp"

// f(x) = x³ + 4x² - 4x + 1, x pertenece [-5, 3]
double cost_function_a(double x, double) {
	return ( x*x*x + 4*x*x - 4*x + 1 );
}

// f(x) = x⁴ + 5x³ + 4x² - 4x + 1, x pertenece {-5, 3]
double cost_function_b(double x, double) {
	return ( x*x*x*x + 5*x*x*x + 4*x*x - 4*x + 1 );
}

// f(x, y) = e - 20exp {-20sqrt[(x² + y²)/2] - exp {[cos(2pix) + cos(2piy)]/2}, x, y pertenecen [-5, 5]
double cost_function_c(double x, double y)
{
    const double e  = std::numbers::e;
    const double pi = std::numbers::pi;

    double term1 = -20.0 * std::exp( -20.0 * std::sqrt((x*x + y*y) / 2.0) );

    double term2 = -std::exp( (std::cos(2*pi*x) + std::cos(2*pi*y)) / 2.0 );

    return e + term1 + term2;
}

Domains dom_a = {
  { -5.0, 3.0 },
  { 0.0, 0.0 }
};

Domains dom_b = {
  { -5.0, 5.0 },
  { -5.0, 5.0 }
};

std::vector<GAConfig> configs = {
    {0.01, 4}, {0.03, 4}, {0.05, 4},
    {0.01, 8}, {0.03, 8}, {0.05, 8}
};

std::vector<GA> ga_builder(size_t pop_size, Fitness cost, const Domains& dom, const std::vector<GAConfig>& configs)
{
  std::vector<GA> gas;
  gas.reserve( configs.size() );
  
  for ( const auto& c : configs ) {
    gas.emplace_back(pop_size, cost, c.mu, dom, c.bits);
    gas.back().initialize();
  }
  
  return gas;
}

int main()
{
  constexpr size_t n_pop(50);
  
  std::vector<GA> ga1( ga_builder(n_pop, cost_function_a, dom_a, configs) );
  std::vector<GA> ga2( ga_builder(n_pop, cost_function_b, dom_a, configs) );
  std::vector<GA> ga3( ga_builder(n_pop, cost_function_c, dom_b, configs) );
  
for (const auto& ga : ga1)
{
    std::cout << "\n=== GA | mu=" << ga.getMutationRate() << " ===\n";

    for (const auto& ind : ga.getPopulation())
    {
        std::cout
            << "x=" << ind.getDecodedX()
            << " cost=" << ind.getCost()
            << '\n';
    }
}

for (const auto& ga : ga1)
{
    std::cout << "\n=== CHECK cost_function_a ===\n";

    for (const auto& ind : ga.getPopulation())
    {
        double manual = cost_function_a(ind.getDecodedX(), 0);

        std::cout
            << "calc=" << ind.getCost()
            << " manual=" << manual
            << '\n';
    }
}

for (auto& ga : ga1)
{
    ga.sort();

    const auto& best = ga.getPopulation()[0];

    std::cout
        << "\nBest (mu=" << ga.getMutationRate() << ") "
        << "x=" << best.getDecodedX()
        << " cost=" << best.getCost()
        << '\n';
}

for (auto& ga : ga1)
{
    ga.sort();

    const auto& pop = ga.getPopulation();

    std::cout
        << "mu=" << ga.getMutationRate()
        << " first=" << pop[0].getCost()
        << " last="  << pop[pop.size()-1].getCost()
        << '\n';
}


  return 1;
}
