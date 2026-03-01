#pragma once  // This will be a common patern in classes, the indication to compiler to only call this file once in every program iteration
#include <random>
#include <algorithm>

class Gene 
{
  private:
    // Gene knows its values and limits of generation (limits).
    double value{};
    double min_value{};
    double max_value{};

  public:
    ~Gene() = default;
    constexpr Gene() noexcept = default;
    explicit constexpr Gene(double val, double min, double max) 
        : value(val), min_value(min), max_value(max) {}

    double getValue() const noexcept { return value; }
    double getMin() const noexcept { return min_value; }
    double getMax() const noexcept { return max_value; }
    
    void setValue(const double new_value) noexcept { value = new_value; }
    
    void randomize(std::mt19937& rng) noexcept {
        std::uniform_real_distribution<double> dist(min_value, max_value);
        value = dist(rng);
    }
    
    // This functions recibes a random generator, the mutation_rate and mutation_scale (noise); for Gauss mutation.
    void mutate(std::mt19937& rng, double mutation_rate, double mutation_scale) noexcept
    {
        std::uniform_real_distribution<double> prob(0.0, 1.0);  // Probability to not mutate.
      
        if ( prob(rng) < mutation_rate ) {
          std::normal_distribution<double> noise(0.0, mutation_scale);
          value += noise(rng);
          value = std::clamp(value, min_value, max_value);
        }
    }
};
