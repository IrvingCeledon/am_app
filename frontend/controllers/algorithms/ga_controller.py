import logging

from frontend.native import ga_module
from .base_controller import BaseController


logger = logging.getLogger(__name__)


class GAController(BaseController):
    def __init__(self):     
        #super().__init__()
        self.config = ga_module.GAConfig()

    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)

        self.config.selectionRate = p["selection"]
        self.config.crossoverRate = p["crossover"]
        self.config.mutationRate = p["mutation"]
        
        self.config.bits = p["bits"]

    def run_with_params(self, params: dict):
        logger.debug("Updating GA config")
        self._update_config(params)

        self.ga = ga_module.GAEngine(self.config)
        
        return self.ga.run()
