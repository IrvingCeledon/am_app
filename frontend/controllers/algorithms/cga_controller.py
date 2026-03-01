import logging
logger = logging.getLogger(__name__)

# Importamos los nuevos módulos distribuidos
from frontend.native import cga_module, core_module

from .base_controller import BaseController

class CGAController(BaseController):
    def __init__(self):        
        self.config = cga_module.CGAConfig()
        # Actualizado a CGAEngine
        self.cga = cga_module.CGAEngine(self.config)
        
    def _update_config(self, p: dict):
        self.config.generations = p["generations"]
        self.config.populationSize = p["population"]

        # Creamos y asignamos el Domains del core
        dom = core_module.Domains()
        dom.ranges = self._build_ranges(p)
        self.config.domains = dom

        self.config.fitness = BaseController.FITNESS_MAP[p["fitness"]]

        # Las tasas ahora están aplanadas en el config (camelCase)
        self.config.selectionRate = p["selection"]
        self.config.crossoverRate = p["crossover"]
        self.config.mutationRate = p["mutation"]
        
        # ==========================================
        # NUEVO: Parámetros de Criterio de Paro
        # ==========================================
        self.config.use_target = p.get("use_target", False)
        self.config.target_fitness = p.get("target_fitness", 0.0001)
        
        self.config.use_stagnation = p.get("use_stagnation", False)
        self.config.stagnation_patience = p.get("stagnation_patience", 20)
        
        self.config.mutationScale = p["mutation_scale"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating CGA config")
        self._update_config(params)

        # Actualizado a CGAEngine
        self.cga = cga_module.CGAEngine(self.config)
        return self.cga.run()
