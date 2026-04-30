import logging
from frontend.native import bfoa_module
from .base_controller import BaseController

logger = logging.getLogger(__name__)

class BFOAController(BaseController):
    def __init__(self):
        super().__init__()
        self.config = bfoa_module.BFOAConfig()

    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)

        self.config.chemotactic_steps = p["chemotactic_steps"]
        self.config.reproduction_steps = p["reproduction_steps"]
        self.config.elimination_dispersal_steps = p["elimination_dispersal_steps"]

        self.config.step_size = p["step_size"]
        self.config.run_length = p["run_length"]
        self.config.elimination_rate = p["elimination_rate"]

        self.config.d_attract = p["d_attract"]
        self.config.w_attract = p["w_attract"]
        self.config.h_repel = p["h_repel"]
        self.config.w_repel = p["w_repel"]

    def run_with_params(self, params: dict):
        logger.debug("Updating BFOA config")
        self._update_config(params)

        self.bfoa = bfoa_module.BFOAEngine(self.config)
        return self.bfoa.run()
