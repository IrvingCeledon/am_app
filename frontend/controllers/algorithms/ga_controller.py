import logging
logger = logging.getLogger(__name__)

from frontend.native import ga_module

class GAController:
    def __init__(self):        
        self.config = ga_module.GAConfig()
        self.ga = ga_module.GA(self.config)
        
    def _update_config(self, p: dict):
        fitness_map = {
            "f1": ga_module.f1,
            "f2": ga_module.f2,
            "f3": ga_module.f3
        }

        self.config.generations = p["generations"]
        self.config.populationSize = p["population"]
        self.config.bits = p["bits"]

        self.config.domains.x.min = p["x_min"]
        self.config.domains.x.max = p["x_max"]
        self.config.domains.y.min = p["y_min"]
        self.config.domains.y.max = p["y_max"]

        self.config.fitness = fitness_map[p["fitness"]]

        self.config.rates.selection = p["selection"]
        self.config.rates.crossover = p["crossover"]
        self.config.rates.mutation = p["mutation"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating GA config")
        self._update_config(params)

        self.ga = ga_module.GA(self.config)
        
        return self.ga.run()
