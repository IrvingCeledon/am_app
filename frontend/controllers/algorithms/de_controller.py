import logging
from frontend.native import de_module
from .base_controller import BaseController

logger = logging.getLogger(__name__)

class DEController(BaseController):
    def __init__(self):
        super().__init__()
        self.config = de_module.DEConfig()

    def _update_config(self, p: dict):
        self._update_base_config(self.config, p)
        self.config.F = p["F"]
        self.config.CR = p["CR"]

    def run_with_params(self, params: dict):
        logger.debug("Updating DE config")
        self._update_config(params)

        self.de = de_module.DEEngine(self.config)
        return self.de.run()
