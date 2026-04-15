#pragma once

#include <random>
#include "../../evo_core/real_individual.hpp"
#include "../engine_base.hpp"
#include "de_config.hpp"

class DEEngine : public EngineBase<RealIndividual, DEConfig>
{
protected:
    void initialize() override;
    void perform_iteration() override;
    void save_history(std::vector<Genome>& target) override;

private:
    std::mt19937 rng{ std::random_device{}() };
    void clamp_individual(RealIndividual& ind);

    // Canonical DE Phases
    Genome mutate(size_t target_idx);
    Genome recombine(const Genome& target, const Genome& mutant);
    void select(Population<RealIndividual>& next_gen, size_t target_idx, RealIndividual& trial);

public:
    explicit DEEngine(const DEConfig& config)
    : EngineBase<RealIndividual, DEConfig>(config) {}
};
