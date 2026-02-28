import sys
import logging
from pathlib import Path

from PySide6.QtWidgets import QApplication
from frontend.controllers.app_controller import AppController


BASE_DIR = Path(__file__).parent.parent
DATA_DIR = BASE_DIR / "data"
LOG_PATH = DATA_DIR / "app.log"


def setup_logging(debug: bool) -> None:
    DATA_DIR.mkdir(exist_ok=True)

    stream_handler = logging.StreamHandler()
    stream_handler.setLevel(logging.DEBUG if debug else logging.INFO)

    file_handler = logging.FileHandler(LOG_PATH, encoding="utf-8")
    file_handler.setLevel(logging.DEBUG)

    logging.basicConfig(
        level=logging.INFO, 
        format="%(asctime)s | %(levelname)s | %(name)s | %(message)s",
        handlers=[stream_handler, file_handler]
    )

    app_logger = logging.getLogger("frontend")
    app_logger.setLevel(logging.DEBUG if debug else logging.INFO)


def main():
    debug_mode = "--debug" in sys.argv

    setup_logging(debug_mode)

    logger = logging.getLogger(__name__)
    logger.info("Executable: %s", sys.executable)
    logger.info("Prefix: %s", sys.prefix)
    logger.info("Debug mode: %s", debug_mode)

    app = QApplication(sys.argv)
    app.setStyle("Fusion")

    controller = AppController(app, BASE_DIR)
    controller.start()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
