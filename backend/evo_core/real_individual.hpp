#pragma once
#include <vector>

class RealIndividual
{
private:
    std::vector<double> chromosome; 
    double cost{};                 

public:
    RealIndividual() = default;
    
    explicit RealIndividual(size_t dims) 
        : chromosome(dims, 0.0) {}

    // Pure Getters / Setters (Reference returns for in-place modifications)
    std::vector<double>& genes() noexcept { return chromosome; }
    const std::vector<double>& genes() const noexcept { return chromosome; }

    double getCost() const noexcept { return cost; }
    void setCost(double new_cost) noexcept { cost = new_cost; }

    // Required for Population::best() and std::sort functionality
    bool operator < (const RealIndividual& other) const noexcept {
        return cost < other.cost;
    }
};
