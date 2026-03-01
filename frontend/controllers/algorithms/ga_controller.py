import logging
logger = logging.getLogger(__name__)

# Importamos los nuevos módulos distribuidos
from frontend.native import ga_module
from .base_controller import BaseController

class GAController(BaseController):
    def __init__(self):     
        #super().__init__()
        self.config = ga_module.GAConfig()
        # Actualizado a GAEngine
        self.ga = ga_module.GAEngine(self.config)
        
    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)

        # Las tasas ahora están aplanadas en el config (camelCase)
        self.config.selectionRate = p["selection"]
        self.config.crossoverRate = p["crossover"]
        self.config.mutationRate = p["mutation"]
        
        self.config.bits = p["bits"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating GA config")
        self._update_config(params)

        # Actualizado a GAEngine
        self.ga = ga_module.GAEngine(self.config)
        
        return self.ga.run()
