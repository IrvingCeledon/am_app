#pragma once
#include <vector>
#include <stdexcept>
#include <random>
#include "gene.hpp"
#include "cga_types.hpp"

class Individual
{
  private:
    std::vector<Gene> chromosome; // A chromosome is a sequential of genes.
    double cost{};  // And one attribute to saves cost value.
    
  public:
    ~Individual() = default;    
    Individual() noexcept = default; 
    
    explicit Individual(const std::vector<Gene>& genes) noexcept
      : chromosome(genes)
      {}
    
    // Getters:
    const std::vector<Gene>& getChromosome() const noexcept { return chromosome; }
    std::vector<Gene>& getChromosome() noexcept { return chromosome; }
    double getCost() const noexcept { return cost; }
    
    Gene& getGene(size_t index) { return chromosome.at(index); }
    const Gene& getGene(size_t index) const { return chromosome.at(index); }
        
    // Setters:
    void setChromosome(const std::vector<Gene>& genes) noexcept { chromosome = genes; }
    void setGene(size_t index, const Gene& newGene) { chromosome.at(index) = newGene; }
    void setCost(double newCost) noexcept { cost = newCost; }
    
    // Others:
    
    void evaluate(const Fitness& fitness) noexcept 
    {
        std::vector<double> values;
        values.reserve(chromosome.size());
        
        for (const auto& gene : chromosome) {
            values.push_back(gene.getValue());
        }
        
        cost = fitness(values);
    }
    
    void mutate(std::mt19937& rng, double mutation_rate, double mutation_scale) noexcept {
        for (auto& gene : chromosome) {
            gene.mutate(rng, mutation_rate, mutation_scale);
        }
    }
    
    // Overload of operator to comparations with sort().
    bool operator < (const Individual& other) const noexcept {
      return cost < other.cost;
    }
};
