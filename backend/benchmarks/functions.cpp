#include <vector>
#include <cmath>
#include <numbers>

#include "functions.hpp"

// f(x) = x³ + 4x² - 4x + 1
// Range: [-5, 3]
double cost_function_a(const Genome& g) {
    double x = g[0]; // Retrieve x from the genome
    return ( x*x*x + 4.0*x*x - 4.0*x + 1.0 );
}

// f(x) = x⁴ + 5x³ + 4x² - 4x + 1
// Range: [-5, 3]
double cost_function_b(const Genome& g) {
    double x = g[0]; // Retrieve x from the genome
    return ( x*x*x*x + 5.0*x*x*x + 4.0*x*x - 4.0*x + 1.0 );
}

// f(x, y)
// Range: [-5, 5] for x and y
double cost_function_c(const Genome& g) 
{
    double x = g[0];
    double y = g[1];

    const double e  = std::numbers::e;
    const double pi = std::numbers::pi;

    double term1 = -20.0 * std::exp( -20.0 * std::sqrt((x*x + y*y) / 2.0) );
    double term2 = -std::exp( (std::cos(2.0*pi*x) + std::cos(2.0*pi*y)) / 2.0 );

    return e + term1 + term2;
}

// f(x, y) = e - 20 *exp(-0.2 * √((x^2 + y^2)/2)) - exp((cos(2*pi*x) + cos(2*pi*y))/2)
// Range: [-5, 5] for x and y
double ackley_function(const std::vector<double>& genes) 
{
    double x = genes.at(0);
    double y = genes.at(1);

    const double e  = std::numbers::e;
    const double pi = std::numbers::pi;

    double term1 = -20.0 * std::exp( -.2 * std::sqrt((x*x + y*y) / 2.0) );

    double term2 = -std::exp( (std::cos(2.0*pi*x) + std::cos(2.0*pi*y)) / 2.0 );

    return e + term1 + term2;
}

// Sphere: f(x) = sum(x[i]^2) for i in 1..n
// Range: [-5.12, 5.12] for every variable, n = 4 && n = 6
double sphere_function(const std::vector<double>& genes)
{
    double sum = 0.0;
    
    for (double x : genes) {
        sum += x * x;
    }
    
    return sum;
}

// Eggholder: f(x, y) = -(y + 47) * sin(√(|y + x/2 +47|)) - x * sin(√(|x - (y + 47)|))
// Range: [-512, 512] for x and y
double eggholder_function(const std::vector<double>& genes)
{
    double x = genes.at(0);
    double y = genes.at(1);
    
    double term1 = - (y + 47.0) * std::sin( std::sqrt( std::abs(y + x/2.0 + 47.0) ) );

    double term2 = - x * std::sin( std::sqrt( std::abs(x - (y + 47.0)) ) );
    
    return term1 + term2;
}

// Rosenbrock: f(x, y) = (a - x)² + b * (y - x²)² , with a = 1 and b = 100
// Range: [-5, 10] for x and y
double rosenbrock_function(const std::vector<double>& genes) 
{
    double x = genes.at(0);
    double y = genes.at(1);
    
    double a(1.0), b(100.0);
    
    return (a - x)*(a - x) + b * ( (y - x*x) * (y - x*x) );
}

inline double pow20(double base)
{
    double base2 = base * base;     // base^2
    double base4 = base2 * base2;   // base^4
    double base5 = base4 * base;    // base^5
    double base10 = base5 * base5;  // base^10
    return base10 * base10;         // base^20
}

// Michalewicz: f(x) = sum(sin(x[i]) * (sin((i*x[i]*x[i])/pi))^2*m) for i in 1..n
// Range: [0, pi] for all dimensions
double michalewicz_function(const std::vector<double>& genes)
{    
    double sum = 0.0;
    // pow20 = ...2*m
    const double pi = std::numbers::pi;

    for (size_t i = 0; i < genes.size(); ++i) 
    {
        double x_i = genes[i];
        
        double multiplier = ( (i + 1.0) * x_i * x_i ) / pi;
        
        sum += std::sin(x_i) * pow20(std::sin(multiplier));
    }

    return -sum;
}

// Six-Hump Camel:
// f(x1, x2) = (4 - 2.1*x1^2 + x1^4/3)*x1^2 + x1*x2 + (4*x2^2 - 4)*x2^2
// Range: x1 ∈ [-3, 3], x2 ∈ [-2, 2]
double six_hump_camel_function(const std::vector<double>& genes)
{
    double x = genes.at(0);
    double y = genes.at(1);

    double x2 = x * x;
    double x4 = x2 * x2;
    double y2 = y * y;

    double term1 = (4.0 - 2.1 * x2 + (x4 / 3.0)) * x2;
    double term2 = x * y;
    double term3 = (4.0 * y2 - 4.0) * y2;

    return term1 + term2 + term3;
}
