#pragma once
#include <vector>

class Bacterium {
private:
    std::vector<double> position;
    double cost{};
    double health{}; // Accumulated fitness for reproduction phase

public:
    Bacterium() = default;
    explicit Bacterium(size_t dims) : position(dims, 0.0) {}

    std::vector<double>& genes() noexcept { return position; }
    const std::vector<double>& genes() const noexcept { return position; }

    double getCost() const noexcept { return cost; }
    void setCost(double new_cost) noexcept { cost = new_cost; }

    double getHealth() const noexcept { return health; }
    void setHealth(double new_health) noexcept { health = new_health; }
    void addHealth(double val) noexcept { health += val; }
    void resetHealth() noexcept { health = 0.0; }

    // Required to sort the population during the reproduction phase
    bool operator < (const Bacterium& other) const noexcept {
        return health < other.health;
    }
};
