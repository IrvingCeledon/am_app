from pathlib import Path

import logging
logger = logging.getLogger(__name__)

from frontend.paths import STYLES_DIR
from models import AppState
from frontend.utils import load_stylesheet
from .settings_controller import SettingsController
from .run_controller import RunController
from frontend.pages import MainWindow

class AppController:
    def __init__(self, app, root_path: Path):
        self.app = app
        self.root_path = root_path 

    def _load_styles(self):
        load_stylesheet(
            self.app,
            STYLES_DIR / "base.qss",
            STYLES_DIR / "buttons.qss",
            STYLES_DIR / "theme_light.qss",
        )
        
    def start(self):
        self._load_styles()
        
        state = AppState()
        
        settings_controller = SettingsController(state)
        run_controller = RunController(state)
        
        self.main_window = MainWindow(
            state,
            settings_controller,
            run_controller
        )
        
        self.main_window.show()
