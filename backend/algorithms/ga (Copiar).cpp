#include "ga.hpp"

// Number of bits / lower_limit / upper_limit
void randomGenerator(int size, std::vector<int>& array, int bits)
{
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> distribution(0, (1<<bits) - 1);

    for( int i(0); i < size; i++ ) {
      array[i] = distribution(generator);
    }
}

void decoder(std::vector<int>& random_array, std::vector<double>& decoded_array, int n_pop, int m, int a, int b) 
{  
  double range = b - a;
  int mask = (1<<m) - 1;
  
    for ( int i(0); i < n_pop; i++ ) {
      decoded_array[i] = a + random_array[i] * ( range / mask );
    }
}

// f(x) = x³ + 4x² - 4x + 1, x pertenece [-5, 3]
double cost_function_a(double x) {
	return ( x*x*x + 4*x*x - 4*x + 1 );
}

// f(x) = x⁴ + 5x³ + 4x² - 4x + 1, x pertenece {-5, 3]
double cost_function_b(double x) {
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

int main()
{
  int m1(4), m2(8); // Number of bit per gen
  int n_pop(50);  // population size
  double mu1(0.01), mu2(0.03), mu3(0.05); // mutation rate
  
  std::vector<int> random_x(n_pop);
  std::vector<double> decoded_x(n_pop); 
  std::vector<double> cost(n_pop);
  
  randomGenerator(n_pop, random_x, m1);
  decoder(random_x, decoded_x, n_pop, m1, -5, 3);
  
  for ( int i(0); i < n_pop; i++ ) {
    std::cout << decoded_x[i] << " -> "
              << cost_function_a(decoded_x[i]) << '\n';
  }
  
  for ( int i(0); i < n_pop; i++ ) {
    cost[i] = cost_function_a(decoded_x[i]);
  }
  
  std::sort(cost.begin(), cost.end());

  for (double c : cost)
    std::cout << c << " ";

  return 1;
}

/*
int main()
{
  int m1(4), m2(8); // Number of bit per gen
  int n_pop(50);  // population size
  double mu1(0.01), mu2(0.03), mu3(0.05); // mutation rate
  
  std::vector<int> random_x(n_pop);
  std::vector<double> decoded_x(n_pop); 
  std::vector<double> cost(n_pop);
  
  randomGenerator(n_pop, random_x, m1);
  decoder(random_x, decoded_x, n_pop, m1, -5, 3);
  
  for ( int i(0); i < n_pop; i++ ) {
    std::cout << decoded_x[i] << " -> "
              << cost_function_a(decoded_x[i]) << '\n';
  }
  
  for ( int i(0); i < n_pop; i++ ) {
    cost[i] = cost_function_a(decoded_x[i]);
  }
  
  std::sort(cost.begin(), cost.end());

  for (double c : cost)
    std::cout << c << " ";

  return 1;
}
*/
