#pragma once

#include <random>
#include "../../evo_core/real_individual.hpp"
#include "../engine_base.hpp"
#include "cga_config.hpp"

class CGAEngine : public EngineBase<RealIndividual, CGAConfig>
{
protected:
    void initialize() override;
    void evaluate() override;
    void selection() override;
    void crossover() override;
    void mutate() override;
    void save_history(std::vector<Genome>& target) override;

private:
    std::mt19937 rng{ std::random_device{}() };

    // Internal helper to keep values ​​within limits
    void clamp_individual(RealIndividual& ind);

public: 
    explicit CGAEngine(const CGAConfig& config) 
        : EngineBase<RealIndividual, CGAConfig>(config) {}
};
