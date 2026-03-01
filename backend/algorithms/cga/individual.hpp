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
    
    explicit Individual(size_t n) : chromosome(n) {}
    
    explicit Individual(const std::vector<Gene>& genes) noexcept
      : chromosome(genes)
      {}
    
    // Getters:
    const std::vector<Gene>& getChromosome() const noexcept { return chromosome; }
    std::vector<Gene>& getChromosome() noexcept { return chromosome; }
    
    Gene& getGene(size_t index) { return chromosome.at(index); }
    const Gene& getGene(size_t index) const { return chromosome.at(index); }
    
    double getCost() const noexcept { return cost; }    
    
    // Setters:
    void setChromosome(const std::vector<Gene>& genes) noexcept { chromosome = genes; }
    void setGene(size_t index, const Gene& newGene) { chromosome.at(index) = newGene; }
    
    void setCost(double newCost) noexcept { cost = newCost; }
    
    // Others:
    
    void randomize(const Domains& domains, std::mt19937& rng)
    {
        chromosome.clear();
        chromosome.reserve(domains.ranges.size());

        for (const auto& r : domains.ranges)
        {
            Gene g(0.0, r.min, r.max);
            g.randomize(rng);
            chromosome.push_back(g);
        }
    }

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
    
    Individual crossoverWith(
        const Individual& mate,
        std::mt19937& rng,
        double rate) const
    {
        std::uniform_real_distribution<double> prob(0.0, 1.0);

        Individual child;
        child.chromosome.reserve(chromosome.size());

        for (size_t i = 0; i < chromosome.size(); ++i)
        {
            const auto& g1 = chromosome[i];
            const auto& g2 = mate.chromosome[i];

            double value = (prob(rng) < rate)
                ? 0.5 * (g1.getValue() + g2.getValue())
                : g1.getValue();

            child.chromosome.emplace_back(value, g1.getMin(), g1.getMax());
        }

        return child;
    }
    
    size_t size() const noexcept { return chromosome.size(); }
    
    // Overload of operator to comparations with sort().
    bool operator < (const Individual& other) const noexcept {
      return cost < other.cost;
    }
};
