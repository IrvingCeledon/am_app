from pathlib import Path


BASE_DIR = Path(__file__).resolve().parent.parent

DATA_DIR = BASE_DIR / "data"
STYLES_DIR = BASE_DIR / "frontend" / "styles"
LOG_PATH = DATA_DIR / "app.log"
