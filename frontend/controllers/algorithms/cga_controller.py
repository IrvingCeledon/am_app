import logging
logger = logging.getLogger(__name__)

# Importamos los nuevos módulos distribuidos
from frontend.native import cga_module
from .base_controller import BaseController

class CGAController(BaseController):
    def __init__(self):     
        super().__init__()
        self.config = cga_module.CGAConfig()
        # Actualizado a CGAEngine
        self.cga = cga_module.CGAEngine(self.config)
        
    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)
        
        # Las tasas ahora están aplanadas en el config (camelCase)
        self.config.selectionRate = p["selection"]
        self.config.crossoverRate = p["crossover"]
        self.config.mutationRate = p["mutation"]
        
        self.config.mutationScale = p["mutation_scale"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating CGA config")
        self._update_config(params)

        # Actualizado a CGAEngine
        self.cga = cga_module.CGAEngine(self.config)
        return self.cga.run()
