#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <functional> 

/*
class Chromosome
class Population
class GeneticAlgorithm

*/

class Chromosome 
{
  private:
    uint16_t x_gene;
    uint16_t y_gene;

  public:
    ~Chromosome() = default;
    Chromosome() = default;
    Chromosome(uint16_t x, uint16_t y) : x_gene(x), y_gene(y) {}

    const uint16_t getX() const { return x_gene; }
    const uint16_t getY() const { return y_gene; }
    
    void setX(const uint16_t new_gene) { x_gene = new_gene; }
    void setY(const uint16_t new_gene) { y_gene = new_gene; }
    
    // void setX(const uint16_t new_gene) { x_gene = new_gene; }
    // void setY(const uint16_t new_gene) { y_gene = new_gene; }
    
    void mutateX(unsigned bit) {
      x_gene ^= (uint16_t(1) << bit);
    }
    
    void mutateY(unsigned bit) {
      y_gene ^= (uint16_t(1) << bit);
    }
};

class Individual
{
  private:
    Chromosome chromosome;
    double decoded_x = 0;
    double decoded_y = 0;
    double cost = 0;
    
  public:
    ~Individual() = default;
    Individual() = default; 
    Individual(Chromosome new_chromo, double new_x, double new_y, double new_cost) 
      : chromosome(new_chromo),
        decoded_x(new_x),
        decoded_y(new_y),
        cost(new_cost)
      {}
    
    const Chromosome& getChromosome() const { return chromosome; }
    Chromosome& getChromosome() { return chromosome; }
    const double getDecodedX() const { return decoded_x; } 
    const double getDecodedY() const { return decoded_y; } 
    const double getCost() const { return cost; }
    
    void setChromosome(const Chromosome& new_chromo) { chromosome = new_chromo; }
    void setDecodedX(const double newX) { decoded_x = newX; }
    void setDecodedY(const double newY) { decoded_y = newY; }
    void setCost(const double newCost) { cost = newCost; }
    
    void decode(double xmin, double xmax, double ymin, double ymax, int bits) {
      double x_range = xmax - xmin;
      double y_range = ymax - ymin;
      
      int mask = (1 << bits) - 1;
      
      decoded_x = xmin + chromosome.getX() * ( x_range / mask );
      decoded_y = ymin + chromosome.getY() * ( y_range / mask );
    }
    
    void evaluate( double(*f)(double, double) ) {
      cost = f(decoded_x, decoded_y);
    }
    
    bool operator < (const Individual& other) const {
      return cost < other.cost;
    }
};

class Population
{
  private:
    std::vector<Individual> individuals;
    
  public: 
    ~Population() = default;
    Population() = default;
    Population(size_t size) : individuals(size) {}
    
    std::vector<Individual>& getIndividuals() { return individuals; }
    const std::vector<Individual>& getIndividuals() const { return individuals; }
    
    auto begin() { return individuals.begin(); }
    auto begin() const { return individuals.begin(); }
    auto end()   { return individuals.end(); }
    auto end()   const { return individuals.end(); }
    size_t size() const { return individuals.size(); }
    Individual& operator[](size_t i) { return individuals[i]; }
    const Individual& operator[](size_t i) const { return individuals[i]; }
};

struct Range {
  double min;
  double max;
};

struct Domains {
  Range x;
  Range y;
};

struct GAConfig {
  double mu;
  int bits; 
};

using Fitness = double(*)(double, double);

class GA
{
  private:
    std::mt19937 rng;
  
    Population pop;
    const Fitness fitness;
    
    // Mutation rate:
    double mutation_rate = 0.01;
    
    const Domains domains;
    const int bits = 4;
    
    uint16_t randGene() {
      std::uniform_int_distribution<uint16_t> dist(0, (1u << bits) - 1);
      return dist(rng);
    }
    
    size_t original_size;
    
  public: 
    ~GA() = default;
    GA() = default;
    GA(size_t popSize, 
       const Fitness f, 
       double mu, 
       Domains dom, 
       int b)
      : pop(popSize), 
        fitness(f), 
        mutation_rate(mu),
        domains(dom),
        bits(b),
        rng(std::random_device{}())
        {}
        
    Population& getPopulation() { return pop; }  
    double getMutationRate() const { return mutation_rate; }
    
    const Population& getPopulation() const { return pop; }
    void setMutationRate(double m) { mutation_rate = m; }
    
    void initialize()
    {
      for ( auto& individual : pop )
      {
        // Generete random chromosomes
        individual.getChromosome().setX(randGene());
        individual.getChromosome().setY(randGene());
        
        // Decode
        individual.decode(domains.x.min, domains.x.max,
                          domains.y.min, domains.y.max,
                          bits);
        
        // Evaluate
        individual.evaluate(fitness);
      }
    }
    
    // I will have to develope more this one:
    void evaluate();
    
    void sort() {
      std::sort( pop.begin(), pop.end() );
    }
    
    void select() {
      double rate = 0.3;
      original_size = pop.size();
      size_t Nkeep = original_size * rate;
      
      pop.getIndividuals().resize(Nkeep);
    }
    
    void crossover()
    {
      size_t Nkeep = pop.size();
      size_t target = original_size;
      
      std::uniform_int_distribution<int> point_dist(1, bits-1);
      
      std::vector<double> weights;
      for ( size_t i = 0; i < Nkeep; ++i ) {
        weights.push_back(Nkeep - i);
      }
      
      std::discrete_distribution<size_t> parent_dist( weights.begin(), weights.end() );
      
      while ( pop.size() < target )
      {
        size_t i1 = parent_dist(rng);
        size_t i2 = parent_dist(rng);
        
        const auto& p1 = pop[i1];
        const auto& p2 = pop[i2];
        
        int point = point_dist(rng);
        
        uint16_t mask_low = (1u << point) - 1;
        uint16_t mask_high = ~mask_low;
        
        uint16_t x =
          (p1.getChromosome().getX() & mask_high) |
          (p2.getChromosome().getX() & mask_low);

        uint16_t y =
          (p1.getChromosome().getY() & mask_high) |
          (p2.getChromosome().getY() & mask_low);
        
        Individual child;
        child.getChromosome().setX(x);
        child.getChromosome().setY(y);
        
        child.decode(domains.x.min, domains.x.max,
                     domains.y.min, domains.y.max, bits);
                     
        child.evaluate(fitness);
        pop.getIndividuals().push_back(child);
      }
    }
    
    void mutate()
    {
      size_t pop_size = pop.size();
      if ( pop_size <= 1 ) return;
      
      size_t n_bits = 2 * bits;
      
      size_t mutation = static_cast<size_t>(
        mutation_rate * (pop_size - 1) * n_bits
      );
      
      std::uniform_int_distribution<size_t> ind_dist(1, pop_size-1); // elitism
      std::uniform_int_distribution<int> gene_dist(0, 1);     // x or y
      std::uniform_int_distribution<int> bit_dist(0, bits-1);
      
      for ( size_t k = 0; k > mutation; ++k )
      {
        size_t i = ind_dist(rng);
        
        auto& chromo = pop[i].getChromosome();
        
        int gene = gene_dist(rng);
        int bit = bit_dist(rng);
        
        if ( gene == 0 ) {
          chromo.mutateX(bit);
        } else {
          chromo.mutateY(bit);
        }
      }
    }
};


