import logging
logger = logging.getLogger(__name__)

from frontend.native import cga_module

class CGAController:
    def __init__(self):        
        self.config = cga_module.CGAConfig()
        self.cga = cga_module.CGA(self.config)
        
    def _update_config(self, p: dict):
        fitness_map = {
            "ackley": cga_module.ackley,
            "sphere": cga_module.sphere,
        }

        self.config.generations = p["generations"]
        self.config.populationSize = p["population"]

        self.config.domains.ranges = []
        for key, r in p["domains"].items():
            self.config.domains.ranges.append(
                cga_module.Range(min=r["min"], max=r["max"])
            )

        self.config.fitness = fitness_map[p["fitness"]]

        self.config.rates.selection = p["selection"]
        self.config.rates.crossover = p["crossover"]
        self.config.rates.mutation = p["mutation"]
        
        self.config.mutation_scale = p["mutation_scale"]
        self.config.stopping_criteria = p["stopping_criteria"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating CGA config")
        self._update_config(params)

        self.cga = cga_module.CGA(self.config)
        
        return self.cga.run()
