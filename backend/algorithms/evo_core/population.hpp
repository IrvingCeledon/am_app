#pragma once
#include <vector>
#include <algorithm>

// This class is a glorified container:
template<typename T>
class Population
{
  private:
    std::vector<T> individuals;
    
  public: 
    Population() = default;
    explicit Population(size_t size) : individuals(size) {}
    
    // Iterators:
    auto begin() noexcept { return individuals.begin(); }
    auto end()   noexcept { return individuals.end(); }
    auto begin() const noexcept { return individuals.begin(); }
    auto end()   const noexcept { return individuals.end(); }

    // Capacity:
    size_t size() const noexcept { return individuals.size(); }
    bool empty() const noexcept { return individuals.empty(); }

    // Access:
    T& operator [] (size_t i) noexcept { return individuals[i]; }
    const T& operator [] (size_t i) const noexcept { return individuals[i]; }

    // Modifiers:
    void resize(size_t n) { individuals.resize(n); }
    void push_back(T ind) { individuals.push_back(std::move(ind)); }
    void clear() noexcept { individuals.clear(); }
    
    // Helpers:
    T& best() noexcept { return *std::min_element(begin(), end()); }
    const T& best() const noexcept { return *std::min_element(begin(), end()); }
    void reserve(size_t n) { individuals.reserve(n); }
};
