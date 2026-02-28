#pragma once  // This will be a common patern in classes, the indication to compiler to only call this file once in every program iteration
#include <random>
#include <cstdint>

class Chromosome 
{
  private:
    // The mos important attributes in this class  are the genes, independent of the dimension of the chromosome.
    uint16_t x_gene{};
    uint16_t y_gene{};
    
    // This flip take advantage of xor operation to don't do multiple if's.
    // Depending on the random bit posicion generated, the bit will mutate depending on xor operation result.
    void flipX(unsigned bit) noexcept { x_gene ^= (uint16_t(1) << bit); }
    void flipY(unsigned bit) noexcept { y_gene ^= (uint16_t(1) << bit); }

  public:
    ~Chromosome() = default;
    constexpr Chromosome() noexcept = default;
    explicit constexpr Chromosome(uint16_t x, uint16_t y) : x_gene(x), y_gene(y) {}

    uint16_t getX() const noexcept { return x_gene; }
    uint16_t getY() const noexcept { return y_gene; }
    
    void setX(const uint16_t new_gene) noexcept { x_gene = new_gene; }
    void setY(const uint16_t new_gene) noexcept { y_gene = new_gene; }
    
    // This functions recibes a random generator, the rate of mutation and the number of bits representation.
    void mutate(std::mt19937& rng, double rate, int bits) noexcept
    {
      std::uniform_real_distribution<double> prob(0.0, 1.0);  // Probability to not mutate.
      std::uniform_int_distribution<int> bit_dist(0, bits-1); // Random position between the bits representation range.
      
      // If mutation rate is higher than the probability to not mutate, then mutate the genes on the random bit position generated.
      if (prob(rng) < rate) flipX( bit_dist(rng) );
      if (prob(rng) < rate) flipY( bit_dist(rng) );
    }
};
