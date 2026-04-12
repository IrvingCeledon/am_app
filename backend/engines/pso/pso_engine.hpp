#pragma once

#include <random>
#include "../../evo_core/particle.hpp"
#include "../engine_base.hpp"
#include "pso_config.hpp"

class PSOEngine : public EngineBase<Particle, PSOConfig>
{
protected:
    void initialize() override;
    void perform_iteration() override;
    void save_history(std::vector<Genome>& target) override;

private:
    std::mt19937 rng{ std::random_device{}() };
    Particle gbest;

    void clamp_particle(Particle& p);
    void update_velocity_and_position();

public:
    explicit PSOEngine(const PSOConfig& config)
    : EngineBase<Particle, PSOConfig>(config) {}
};
