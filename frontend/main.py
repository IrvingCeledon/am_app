import sys
import logging
from pathlib import Path

from PySide6.QtWidgets import QApplication
from frontend.paths import BASE_DIR, DATA_DIR, LOG_PATH
from frontend.controllers.app_controller import AppController


def setup_logging(debug: bool) -> None:
    DATA_DIR.mkdir(exist_ok=True)

    formatter = logging.Formatter(
        "%(asctime)s | %(levelname)s | %(name)s | %(message)s"
    )

    stream_handler = logging.StreamHandler()
    stream_handler.setLevel(logging.DEBUG if debug else logging.INFO)
    stream_handler.setFormatter(formatter)

    file_handler = logging.FileHandler(LOG_PATH, encoding="utf-8")
    file_handler.setLevel(logging.DEBUG)
    file_handler.setFormatter(formatter)

    root = logging.getLogger()
    root.setLevel(logging.WARNING) 

    root.addHandler(stream_handler)
    root.addHandler(file_handler)

    # CAMBIO AQUÍ: Activamos explícitamente NUESTROS paquetes
    my_level = logging.DEBUG if debug else logging.INFO
    logging.getLogger("frontend").setLevel(my_level)
    logging.getLogger("services").setLevel(my_level)
    logging.getLogger("__main__").setLevel(my_level) # Para que se vean los logs de la función main()


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
