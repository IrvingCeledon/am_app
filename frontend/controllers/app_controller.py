from pathlib import Path
from frontend.utils import load_stylesheet
from frontend.pages import MainWindow

class AppController:
    def __init__(self, app, root_path: Path):
        self.app = app
        self.root_path = root_path 
        self.frontend_dir = self.root_path / "frontend"
        self.styles_dir = self.frontend_dir / "styles"
        self.data_dir = self.root_path / "data" 

    def start(self):
        self._create_data_folder()
        self._load_styles()
        self._show_main_window()

    def _create_data_folder(self):
        self.data_dir.mkdir(exist_ok=True)
        print(f"Data folder: {self.data_dir}")

    def _load_styles(self):
        load_stylesheet(
            self.app,
            self.styles_dir / "base.qss",
            self.styles_dir / "buttons.qss",
            self.styles_dir / "theme_light.qss",
        )

    def _show_main_window(self):
        self.main_window = MainWindow()
        self.main_window.show()
