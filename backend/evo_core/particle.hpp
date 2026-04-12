#pragma once
#include <vector>

class Particle {
private:
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> pbest_position;

    double cost{};
    double pbest_cost{};

public:
    Particle() = default;
    explicit Particle(size_t dims)
    : position(dims, 0.0), velocity(dims, 0.0), pbest_position(dims, 0.0) {}

    // Alias to match EngineBase and history extraction API
    std::vector<double>& genes() noexcept { return position; }
    const std::vector<double>& genes() const noexcept { return position; }

    std::vector<double>& getVelocity() noexcept { return velocity; }
    const std::vector<double>& getVelocity() const noexcept { return velocity; }

    std::vector<double>& getPBest() noexcept { return pbest_position; }
    const std::vector<double>& getPBest() const noexcept { return pbest_position; }

    double getCost() const noexcept { return cost; }
    void setCost(double new_cost) noexcept { cost = new_cost; }

    double getPBestCost() const noexcept { return pbest_cost; }
    void setPBestCost(double new_cost) noexcept { pbest_cost = new_cost; }

    // Required for EngineBase::pop.best()
    bool operator < (const Particle& other) const noexcept {
        return cost < other.cost;
    }
};
