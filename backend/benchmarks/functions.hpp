#pragma once

#include "../evo_core/types.hpp"

double cost_function_a(const Genome&);
double cost_function_b(const Genome&);
double cost_function_c(const Genome&);

double ackley_function(const std::vector<double>&);
double sphere_function(const std::vector<double>&);

double eggholder_function(const std::vector<double>&);

double rosenbrock_function(const std::vector<double>&);
inline double pow20(double);
double michalewicz_function(const std::vector<double>&);
