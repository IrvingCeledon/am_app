#pragma once
#include <functional>
#include <vector>

using Fitness = std::function<double(const std::vector<double>&)>;

struct Range {
    double min;
    double max;
};

struct Domains {
    std::vector<Range> ranges;
};

struct Rates {
  double selection;
  double crossover;
  double mutation;
};

struct RunResult {
    std::vector<double> bestFitnesses;                  
    std::vector<std::vector<double>> initialPopulation; 
    std::vector<std::vector<double>> midPopulation;  
    std::vector<std::vector<double>> finalPopulation;
};
