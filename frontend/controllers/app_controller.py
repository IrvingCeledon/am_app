from pathlib import Path

import logging
logger = logging.getLogger(__name__)

from frontend.utils import load_stylesheet
from .settings_controller import SettingsController
from .run_controller import RunController
from models import AppState
from frontend.pages import MainWindow

class AppController:
    def __init__(self, app, root_path: Path):
        self.app = app
        self.root_path = root_path 
        self._frontend_dir = self.root_path / "frontend"
        self._styles_dir = self._frontend_dir / "styles"
        self._data_dir = self.root_path / "data" 

    def _create_data_folder(self):
        self._data_dir.mkdir(exist_ok=True)
        logger.info(f"Checking data folder {self._data_dir}")

    def _load_styles(self):
        load_stylesheet(
            self.app,
            self._styles_dir / "base.qss",
            self._styles_dir / "buttons.qss",
            self._styles_dir / "theme_light.qss",
        )
        
    def start(self):
        self._create_data_folder()
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
