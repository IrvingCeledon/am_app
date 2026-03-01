#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include "../evo_core/types.hpp"
#include "../evo_core/population.hpp"

// Template para abstraer el tipo de individuo (Binario o Real) y el tipo de Configuración
template<typename IndividualType, typename ConfigType>
class EngineBase 
{
protected:
    ConfigType configuration;
    Population<IndividualType> pop;

    // ==========================================
    // Interfaz Obligatoria para los Hijos
    // ==========================================
    virtual void initialize() = 0;
    virtual void evaluate() = 0;
    virtual void selection() = 0;
    virtual void crossover() = 0;
    virtual void mutate() = 0;
    virtual void save_history(std::vector<Genome>& target) = 0;

    // ==========================================
    // Lógica Universal Absorbida
    // ==========================================
    bool check_stopping_criteria(RunResult& result, size_t& patience, double& prev_best, double current_best) 
    {
        if (configuration.use_target && current_best <= configuration.target_fitness) {
            push_log(result, LogLevel::INFO_LVL, "Target fitness reached: " + std::to_string(current_best) + " <= " + std::to_string(configuration.target_fitness));
            prev_best = current_best;
            return true;
        }

        if (configuration.use_stagnation) {
            if (prev_best != std::numeric_limits<double>::max()) {
                if (std::abs(prev_best - current_best) < 1e-6) {
                    patience++;
                    if (patience >= configuration.stagnation_patience) {
                        push_log(result, LogLevel::WARNING_LVL, "Early stopping: no improvement for " + std::to_string(configuration.stagnation_patience) + " generations.");
                        return true;
                    }
                    return false; 
                } else {
                    patience = 0; 
                }
            }
        }

        prev_best = current_best;
        return false;
    }

    // Helper de Logging
    void push_log(RunResult& res, LogLevel lvl, const std::string& msg) {
        if (lvl >= configuration.verbosity) {
            res.logs.push_back({lvl, msg});
        }
    }
    
public:
    // Constructor base
    EngineBase(const ConfigType& config) 
        : configuration(config), pop(config.populationSize) {}
        
    // Destructor virtual necesario en herencia C++
    virtual ~EngineBase() = default;

    // ==========================================
    // El Orquestador Principal (Template Method)
    // ==========================================
    RunResult run() 
    {
        RunResult result;

        push_log(result, LogLevel::INFO_LVL, "Initializing population of size : " + std::to_string(configuration.populationSize));

        initialize();
        std::sort(pop.begin(), pop.end());
        save_history(result.initialPopulation);
        
        size_t stagnation_counter = 0;
        double prev_best = std::numeric_limits<double>::max();

        for (size_t g = 0; g < configuration.generations; ++g) 
        {
            selection();
            crossover();
            mutate();
            evaluate();
            std::sort(pop.begin(), pop.end());

            double current_best = pop.best().getCost();
            result.bestFitnesses.push_back(current_best);

            if (g == configuration.generations / 2) {
                save_history(result.midPopulation);
            }

            if (check_stopping_criteria(result, stagnation_counter, prev_best, current_best)) {
                push_log(result, LogLevel::INFO_LVL, "Early stopping at generation: " + std::to_string(g+1));
                break;
            }
        }
        
        push_log(result, LogLevel::INFO_LVL, "Execution completed at generation " + std::to_string(result.bestFitnesses.size()) + ". Best fitness: " + std::to_string(prev_best));

        save_history(result.finalPopulation);
        return result;
    }
};
