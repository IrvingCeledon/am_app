#pragma once
#include "ga_types.hpp"

struct GAConfig
{
    size_t generations;
    size_t populationSize;
    int bits;

    Domains domains;
    Fitness fitness;
    Rates rates;
};
