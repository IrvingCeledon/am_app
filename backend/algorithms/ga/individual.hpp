#pragma once
#include <vector>
#include <utility>
#include "chromosome.hpp"
#include "types.hpp"

class Individual
{
  private:
    Chromosome chromosome{};  // Binary.
    Genome phenotype; // real (R^n)
    double cost{};  // And one attribute to saves cost value.
    
  public:
    ~Individual() = default;    
    Individual() noexcept = default; 
    
    explicit Individual(size_t dims) noexcept
      : chromosome(dims)
      , phenotype(dims)
      {}
    
    // Getters:
    Chromosome& genes() noexcept { return chromosome; }
    const Chromosome& genes() const noexcept { return chromosome; }
    
    const Genome& values() const noexcept { return phenotype; }

    double getCost() const noexcept { return cost; }
    
    // Others:
    // This function follows the binary decoder to real value with lineal scale  function:
    void decode(const Domains& domains, int bits) noexcept
    {
        const double mask = (1u << bits) - 1u;

        for (size_t i = 0; i < chromosome.size(); ++i)
        {
            const auto& r = domains[i];

            phenotype[i] =
                r.min + chromosome[i] * ((r.max - r.min) / mask);
        }
    }
    
    void evaluate(const Fitness& f) noexcept {
        cost = f(phenotype);
    }
    
    // Overload of operator to comparations with sort().
    bool operator < (const Individual& other) const noexcept {
      return cost < other.cost;
    }
};
