import logging

from frontend.native import core_module
from frontend.config import BENCHMARKS


logger = logging.getLogger(__name__)


class BaseController:
    def _build_ranges(self, p: dict):
        # Just in case, we set x_i with 2D
        x_i = p.get("x_i", 2)

        ranges = []
        if x_i == 2:
            ranges.append(core_module.Range(p["x_min"], p["x_max"]))
            ranges.append(core_module.Range(p["y_min"], p["y_max"]))
        else:
            for _ in range(x_i):
                ranges.append(core_module.Range(p["x_min"], p["x_max"]))

        return ranges
        
    def _update_base_config(self, config_obj, p: dict):
        config_obj.generations = p["generations"]
        config_obj.populationSize = p["population"]

        # Creation and assignation of core domains
        dom = core_module.Domains()
        dom.ranges = self._build_ranges(p)
        config_obj.domains = dom

        config_obj.fitness = BENCHMARKS[p["fitness"]]["func"]
        
        # Stopping criteria
        config_obj.use_target = p.get("use_target", False)
        config_obj.target_fitness = p.get("target_fitness", 0.0001)
        
        config_obj.use_stagnation = p.get("use_stagnation", False)
        config_obj.stagnation_patience = p.get("stagnation_patience", 20)
        
        config_obj.minimize = p.get("minimize", True)
        
        if logger.isEnabledFor(logging.DEBUG):
            config_obj.verbosity = core_module.LogLevel.DEBUG
        else:
            config_obj.verbosity = core_module.LogLevel.INFO
