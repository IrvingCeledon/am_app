#pragma once
#include "cga_types.hpp"

struct CGAConfig
{
    size_t generations;
    size_t populationSize;

    Domains domains;
    Fitness fitness;
    Rates rates;
    
    double mutation_scale;
    double stopping_criteria;
};
