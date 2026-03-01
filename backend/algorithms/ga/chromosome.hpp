#pragma once  // This will be a common patern in classes, the indication to compiler to only call this file once in every program iteration
#include <vector>
#include <random>
#include <cstdint>

class Chromosome 
{
  private:
    // The mos important attributes in this class  are the genes, independent of the dimension of the chromosome.
    std::vector<uint16_t> genes;
    
  public:
    ~Chromosome() = default;
    constexpr Chromosome() noexcept = default;
    explicit constexpr Chromosome(size_t dims) 
      : genes(dims, 0) {}

    size_t size() const noexcept { return genes.size(); }
    
    uint16_t operator[](size_t i) const noexcept { return genes[i]; }
    uint16_t& operator[](size_t i) noexcept { return genes[i]; }
    
    const std::vector<uint16_t>& data() const noexcept { return genes; }
    std::vector<uint16_t>& data() noexcept { return genes; }
    
    // This functions recibes a random generator, the rate of mutation and the number of bits representation.
    void mutate(std::mt19937& rng, double rate, int bits) noexcept
    {
      std::uniform_real_distribution<double> prob(0.0, 1.0);  // Probability to not mutate.
      std::uniform_int_distribution<int> bit_dist(0, bits-1); // Random position between the bits representation range.
      
      // If mutation rate is higher than the probability to not mutate, then mutate the genes on the random bit position generated.
      for (auto& g : genes) {
          if (prob(rng) < rate) {
              g ^= (uint16_t(1) << bit_dist(rng));
          }
      }
    }
};
