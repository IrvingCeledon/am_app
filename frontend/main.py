import sys
from pathlib import Path
from PySide6.QtWidgets import QApplication
from frontend.controllers.app_controller import AppController

BASE_DIR = Path(__file__).parent.parent

def main():
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    
    print(sys.executable) 
    print(sys.prefix)

    controller = AppController(app, BASE_DIR)
    controller.start()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()

