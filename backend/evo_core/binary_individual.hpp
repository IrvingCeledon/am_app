#pragma once
#include <vector>
#include <cstdint>

class BinaryIndividual
{
private:
    std::vector<uint32_t> chromosome; // Genotype: the bits used for GA to make crossover/mutation
    std::vector<double> phenotype;    // Fenotype: the values decode to R^n
    double cost{};

public:
    BinaryIndividual() = default;
    
    explicit BinaryIndividual(size_t dims)
        : chromosome(dims, 0), phenotype(dims, 0.0) {}

    // Genotype access (for GAEngine) 
    std::vector<uint32_t>& genes() noexcept { return chromosome; }
    const std::vector<uint32_t>& genes() const noexcept { return chromosome; }

    // Fenotype access (to evaluate fitness and save results) 
    std::vector<double>& values() noexcept { return phenotype; }
    const std::vector<double>& values() const noexcept { return phenotype; }

    double getCost() const noexcept { return cost; }
    void setCost(double new_cost) noexcept { cost = new_cost; }

    bool operator < (const BinaryIndividual& other) const noexcept {
        return cost < other.cost;
    }
};
