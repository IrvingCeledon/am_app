#pragma once

#include <vector>
#include <functional>
#include <string> // Asegúrate de incluir string

// ==========================================
// Logging Types (Matching Python logging levels)
// ==========================================

enum class LogLevel : int {
    DEBUG_LVL = 10,
    INFO_LVL = 20,
    WARNING_LVL = 30,
    ERROR_LVL = 40
};

struct LogEntry {
    LogLevel level;
    std::string message;
};

// ==========================================
// Mathematical Types (Space R^n)
// ==========================================

// Universal fenotype. Every algorithm works or are decoded to R^n. 
using Genome = std::vector<double>;

// Universal target function, f : R^n -> R
using Fitness = std::function<double(const Genome&)>;

// ==========================================
// Search Space Domains
// ==========================================

struct Range {
    double min{};
    double max{};
};

struct Domains {
    std::vector<Range> ranges;

    size_t dimension() const noexcept {
        return ranges.size();
    }

    const Range& operator [] (size_t i) const noexcept {
        return ranges[i];
    }
};

// ==========================================
// Universal Results (Shared by all)
// ==========================================

struct RunResult {
    std::vector<double> bestFitnesses;

    // Population history in phenotypic space (Genome)
    // This causes Pybind11 to expose it directly as list[list[float]]
    std::vector<Genome> initialPopulation;
    std::vector<Genome> midPopulation;
    std::vector<Genome> finalPopulation;
    std::vector<LogEntry> logs;
};
