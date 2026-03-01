import logging
logger = logging.getLogger(__name__)

from frontend.native import cga_module

class CGAController:
    def __init__(self):        
        self.config = cga_module.CGAConfig()
        self.cga = cga_module.CGA(self.config)
        
    def _build_ranges(self, p: dict):
        x_i = p["x_i"]

        ranges = []

        if x_i == 2:
            # Classic 2D case → X && Y
            ranges.append(cga_module.Range(p["x_min"], p["x_max"]))
            ranges.append(cga_module.Range(p["y_min"], p["y_max"]))
        else:
            # N dimensions → repeat X domain
            for _ in range(x_i):
                ranges.append(cga_module.Range(p["x_min"], p["x_max"]))

        return ranges
        
    def _update_config(self, p: dict):
        fitness_map = {
            "ackley": cga_module.ackley,
            "sphere": cga_module.sphere,
        }

        self.config.generations = p["generations"]
        self.config.populationSize = p["population"]

        self.config.domains.ranges = self._build_ranges(p)

        self.config.fitness = fitness_map[p["fitness"]]

        self.config.rates.selection = p["selection"]
        self.config.rates.crossover = p["crossover"]
        self.config.rates.mutation = p["mutation"]
        
        self.config.mutation_scale = p["mutation_scale"]
        # self.config.stopping_criteria = p["stopping"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating CGA config")
        self._update_config(params)

        self.cga = cga_module.CGA(self.config)
        return self.cga.run()
