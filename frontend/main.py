import sys
from pathlib import Path
from PySide6.QtWidgets import QApplication
from frontend.utils.style_loader import load_stylesheet
from .pages.main_window import MainWindow

# BASE_DIR for styles
BASE_DIR = Path(__file__).parent

def main():
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    
    load_styles(app)
    
    w = MainWindow()
    w.setWindowTitle("Metaheuristic Algorithm Simulator V1")
    w.show()
    
    print(sys.executable)
    print(sys.prefix)
    
    sys.exit(app.exec())

# At the moment i'm executing this app from root, with a relative to ".../frontend/...", this paths could change in the future:
def load_styles(app):
    load_stylesheet(
        app,
        BASE_DIR / "styles/base.qss",
        BASE_DIR / "styles/buttons.qss",
        BASE_DIR / "styles/theme_light.qss",
    )
    
if __name__ == "__main__":
    main()
