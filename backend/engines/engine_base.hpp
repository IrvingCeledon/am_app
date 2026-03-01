#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include "../evo_core/types.hpp"
#include "../evo_core/population.hpp"

// ==========================================
// BOAT GOES BINTED!!!!!
// ==========================================
template<typename IndividualType, typename ConfigType>
class EngineBase 
{
protected:
    ConfigType configuration;
    Population<IndividualType> pop;

    virtual void initialize() = 0;
    virtual void perform_iteration() = 0;
    virtual void save_history(std::vector<Genome>& target) = 0;

    double evaluate_fitness(const Genome& g) const {
        double val = configuration.fitness(g);
        return configuration.minimize ? val : -val;
    }

    bool check_stopping_criteria(
        RunResult& result, 
        size_t& patience, 
        double& prev_best, 
        double current_best
    ) 
    {
        // Ajustamos el target según lo que buscamos
        double internal_target = configuration.minimize ? configuration.target_fitness : -configuration.target_fitness;
        
        if (configuration.use_target && current_best <= internal_target) 
        {
            double display_best = configuration.minimize ? current_best : -current_best;
            push_log(result, LogLevel::INFO_LVL,
                    "Target fitness reached: " + std::to_string(display_best));
                    
            prev_best = current_best;
            return true;
        }
        
        if (!configuration.use_stagnation) {
            prev_best = current_best;
            return false;
        }
        
        if (prev_best == std::numeric_limits<double>::max()) {
            prev_best = current_best;
            return false;
        }
        
        const bool improved = prev_best - current_best >= 1e-6;

        if (improved) {
            patience = 0;
            prev_best = current_best;
            return false;
        }
        
        ++patience;
        
        if (patience >= configuration.stagnation_patience) {
            push_log(result, LogLevel::WARNING_LVL,
                     "Early stopping: no improvement for " + 
                     std::to_string(configuration.stagnation_patience) + 
                     " generations.");       
            return true;
        }
        
        return false;
    }

    // Logging "emulator"
    void push_log(RunResult& res, LogLevel lvl, const std::string& msg) {
        if (lvl >= configuration.verbosity) {
            res.logs.push_back({lvl, msg});
        }
    }
    
public:
    EngineBase(const ConfigType& config) 
        : configuration(config), pop(config.populationSize) {}
        
    virtual ~EngineBase() = default;

    RunResult run() 
    {
        RunResult result;

        push_log(result, LogLevel::INFO_LVL, 
                "Initializing population of size : " + 
                std::to_string(configuration.populationSize));

        initialize();
        save_history(result.initialPopulation);
        
        size_t stagnation_counter = 0;
        double prev_best = std::numeric_limits<double>::max();

        for (size_t g = 0; g < configuration.generations; ++g) 
        {
            perform_iteration();

            double current_best = pop.best().getCost();
            
            // --- CAMBIO AQUÍ: Guardamos el valor real para la gráfica ---
            double display_best = configuration.minimize ? current_best : -current_best;
            result.bestFitnesses.push_back(display_best);

            if (g == configuration.generations / 2) {
                save_history(result.midPopulation);
            }

            if (check_stopping_criteria(result, stagnation_counter, prev_best, current_best)) {
                push_log(result, LogLevel::INFO_LVL, 
                        "Early stopping at generation: " + 
                        std::to_string(g+1));
                break;
            }
        }
        
        double final_display = configuration.minimize ? prev_best : -prev_best;
        push_log(result, LogLevel::INFO_LVL, 
                "Execution completed at generation " + 
                std::to_string(result.bestFitnesses.size()) + 
                ". Best fitness: " + std::to_string(final_display));

        save_history(result.finalPopulation);
        
        return result;
    }
};
