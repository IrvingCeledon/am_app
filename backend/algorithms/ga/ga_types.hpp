#pragma once
#include <functional>
#include <vector>
#include <tuple>

using Fitness = std::function<double(double,double)>;

struct Range {
    double min;
    double max;
};

struct Domains {
    Range x;
    Range y;
};

struct Rates {
  double selection;
  double crossover;
  double mutation;
};

struct RunResult {
    std::vector<double> bestFitnesses;                        // Best individual per generation
    std::vector<std::tuple<double,double>> initialPopulation; // (x,y) of initial population
    std::vector<std::tuple<double,double>> midPopulation;     // mid population (N/2)
    std::vector<std::tuple<double,double>> finalPopulation;
};
