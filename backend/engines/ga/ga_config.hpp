#include "../../evo_core/config_base.hpp"

struct GAConfig : EvoConfigBase {
    int bits{};
    double selectionRate{};
    double crossoverRate{};
    double mutationRate{};
};
