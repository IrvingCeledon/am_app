from typing import List, Optional
from models import RunResult

class AppState:
    def __init__(self):
        self.last_run_result: Optional[RunResult] = None
        self.history: List[RunResult] = []
        self.settings = {}
        self.shared_data = {}
