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
