import logging
logger = logging.getLogger(__name__)

from frontend.native import core_module, benchmarks_module

class BaseController:
    # Atributo de clase compartido por todos los hijos (ahorra memoria)
    FITNESS_MAP = {
        "f1": benchmarks_module.cost_function_a,
        "f2": benchmarks_module.cost_function_b,
        "f3": benchmarks_module.cost_function_c,
        "ackley": benchmarks_module.ackley,
        "sphere": benchmarks_module.sphere,
        "eggholder": benchmarks_module.eggholder,
        "rosenbrock": benchmarks_module.rosenbrock,
        "michalewicz": benchmarks_module.michalewicz
    }

    def _build_ranges(self, p: dict):
        # Usamos .get("x_i", 2) por si algún algoritmo (como tu GA original) no envía "x_i"
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

        # Creamos y asignamos el Domains del core
        dom = core_module.Domains()
        dom.ranges = self._build_ranges(p)
        config_obj.domains = dom

        config_obj.fitness = self.FITNESS_MAP[p["fitness"]]
        
        # ==========================================
        # NUEVO: Parámetros de Criterio de Paro
        # ==========================================
        config_obj.use_target = p.get("use_target", False)
        config_obj.target_fitness = p.get("target_fitness", 0.0001)
        
        config_obj.use_stagnation = p.get("use_stagnation", False)
        config_obj.stagnation_patience = p.get("stagnation_patience", 20)
        
        if logger.isEnabledFor(logging.DEBUG):
            config_obj.verbosity = core_module.LogLevel.DEBUG
        else:
            config_obj.verbosity = core_module.LogLevel.INFO
