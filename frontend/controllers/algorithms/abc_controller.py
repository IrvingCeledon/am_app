import logging
from frontend.native import abc_module
from .base_controller import BaseController

logger = logging.getLogger(__name__)

class ABCController(BaseController):
    def __init__(self):     
        super().__init__()
        self.config = abc_module.ABCConfig()

    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)
        self.config.limit = p["limit"]
        
    def run_with_params(self, params: dict):
        logger.debug("Updating ABC config")
        self._update_config(params)

        self.abc = abc_module.ABCEngine(self.config)
        return self.abc.run()
