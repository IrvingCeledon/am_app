#include "../../evo_core/config_base.hpp"

struct CGAConfig : EvoConfigBase {
    double selectionRate{};
    double crossoverRate{};
    double mutationRate{};
    double mutationScale{}; // Standard deviation of Gaussian noise
};
