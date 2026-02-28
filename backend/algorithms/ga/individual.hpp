#pragma once
#include <utility>
#include "chromosome.hpp"

class Individual
{
  private:
    Chromosome chromosome{};  // Each individual has only one chromosome.
    double decoded_x{}; // But have an decoded attribute per gene.
    double decoded_y{};
    double cost{};  // And one attribute to saves cost value.
    
  public:
    ~Individual() = default;    
    Individual() noexcept = default; 
    
    explicit Individual(const Chromosome& new_chromo, 
                        double new_x, 
                        double new_y, 
                        double new_cost) noexcept
      : chromosome(new_chromo),
        decoded_x(new_x),
        decoded_y(new_y),
        cost(new_cost)
      {}
    
    // Getters:
    const Chromosome& getChromosome() const noexcept { return chromosome; }
    Chromosome& getChromosome() noexcept { return chromosome; }
    
    double getDecodedX() const noexcept { return decoded_x; } 
    double getDecodedY() const noexcept { return decoded_y; } 
    double getCost() const noexcept { return cost; }
    
    // Setters:
    void setChromosome(Chromosome new_chromo) noexcept { chromosome = std::move(new_chromo); }
    void setDecodedX(const double newX) noexcept { decoded_x = newX; }
    void setDecodedY(const double newY) noexcept { decoded_y = newY; }
    void setCost(const double newCost) noexcept { cost = newCost; }
    
    // Others:
    // This function follows the binary decoder to real value with lineal scale  function:
    void decode(double xmin, double xmax, 
                double ymin, double ymax, 
                int bits) noexcept 
    {
      const double x_range = xmax - xmin;
      const double y_range = ymax - ymin;
      
      const auto mask = (1u << bits) - 1u;
      
      decoded_x = xmin + chromosome.getX() * ( x_range / mask );
      decoded_y = ymin + chromosome.getY() * ( y_range / mask );
    }
    
    // A template to don't include functional libray and sped proccess.
    template<typename F>
    void evaluate(F&& f) noexcept {
      cost = f(decoded_x, decoded_y);
    }
    
    // Overload of operator to comparations with sort().
    bool operator < (const Individual& other) const noexcept {
      return cost < other.cost;
    }
};
