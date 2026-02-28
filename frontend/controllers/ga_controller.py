# ~/frontend/controller/ga_controller.py
from pathlib import Path
import sys

GA_BUILD_PATH = Path(__file__).resolve().parent.parent.parent / "backend/algorithms/ga/build"
sys.path.insert(0, str(GA_BUILD_PATH))

import ga_module

class GAController:
    def __init__(self, config=None):
        self.config = config or ga_module.GAConfig()
        self.ga = ga_module.GA(self.config)

    def set_parameters_from_widgets(
        self,
        generations_spin,
        population_spin, 
        bits_spin,
        x_min_spin, x_max_spin,
        y_min_spin, y_max_spin,
        fitness_combo,
        selection_spin, crossover_spin, mutation_spin
    ):
    
        # Map intern string to C++ function
        fitness_map = {
            "f1": ga_module.f1,
            "f2": ga_module.f2,
            "f3": ga_module.f3
        }

        fitness_key = fitness_combo.currentData()
        fitness_func = fitness_map[fitness_key]

        self.config.generations = generations_spin.value()
        self.config.populationSize = population_spin.value()
        self.config.bits = bits_spin.value()
        self.config.domains.x.min = x_min_spin.value()
        self.config.domains.x.max = x_max_spin.value()
        self.config.domains.y.min = y_min_spin.value()
        self.config.domains.y.max = y_max_spin.value()
        self.config.fitness = fitness_func
        self.config.rates.selection = selection_spin.value()
        self.config.rates.crossover = crossover_spin.value()
        self.config.rates.mutation = mutation_spin.value()
        
        # Re-creation of GA -> in place of update
        self.ga = ga_module.GA(self.config)

    def run(self):
        return self.ga.run()  
