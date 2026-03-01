import logging
logger = logging.getLogger(__name__)

# Importamos los nuevos módulos distribuidos
from frontend.native import ga_module, core_module

from .base_controller import BaseController

class GAController(BaseController):
    def __init__(self):        
        self.config = ga_module.GAConfig()
        # Actualizado a GAEngine
        self.ga = ga_module.GAEngine(self.config)
        
    def _update_config(self, p: dict):
        self.config.generations = p["generations"]
        self.config.populationSize = p["population"]

        # Usamos el sistema de dominios universales
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
        
        self.config.bits = p["bits"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating GA config")
        self._update_config(params)

        # Actualizado a GAEngine
        self.ga = ga_module.GAEngine(self.config)
        
        return self.ga.run()
