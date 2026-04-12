import logging
from frontend.native import pso_module
from .base_controller import BaseController

logger = logging.getLogger(__name__)

class PSOController(BaseController):
    def __init__(self):
        super().__init__()
        self.config = pso_module.PSOConfig()

    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)
        self.config.w = p["w"]
        self.config.c1 = p["c1"]
        self.config.c2 = p["c2"]

    def run_with_params(self, params: dict):
        logger.debug("Updating PSO config")
        self._update_config(params)

        self.pso = pso_module.PSOEngine(self.config)
        return self.pso.run()
