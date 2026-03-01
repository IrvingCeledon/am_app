import logging

from frontend.native import cga_module
from .base_controller import BaseController


logger = logging.getLogger(__name__)


class CGAController(BaseController):
    def __init__(self):     
        super().__init__()
        self.config = cga_module.CGAConfig()

        self.cga = cga_module.CGAEngine(self.config)
        
    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)
        
        self.config.selectionRate = p["selection"]
        self.config.crossoverRate = p["crossover"]
        self.config.mutationRate = p["mutation"]
        
        self.config.mutationScale = p["mutation_scale"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating CGA config")
        self._update_config(params)

        self.cga = cga_module.CGAEngine(self.config)
        return self.cga.run()
