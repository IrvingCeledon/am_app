#pragma once
#include "../../evo_core/config_base.hpp"

struct ABCConfig : EvoConfigBase {
    size_t limit{100}; // Límite de intentos antes de que una fuente sea abandonada
};
