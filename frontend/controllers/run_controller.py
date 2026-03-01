from PySide6.QtCore import QObject, Signal

import logging
logger = logging.getLogger(__name__)

from frontend.paths import DATA_DIR
from models import RunResult
from services import export_run_txt
from frontend.controllers.algorithms import GAController, CGAController

class RunController(QObject):
    resultReady = Signal(object) 

    def __init__(self, state):
        super().__init__()
        
        self.state = state
        self.algorithms = {
            "GA": GAController(),
            "CGA": CGAController()
        }

    def run(self, algorithm_name: str, params: dict):
        logger.info(f"Running {algorithm_name}")
        logger.debug(f"Received parameters {params}")
        
        if algorithm_name not in self.algorithms:
            raise ValueError(...)

        result = self.algorithms[algorithm_name].run_with_params(params)
        
        for entry in result.logs:
            # Casteamos el Enum de C++ a int (10, 20, 30, 40)
            log_level_int = int(entry.level) 
            logger.log(log_level_int, f"[C++ {algorithm_name}] {entry.message}")
            
        logger.info(f"Execution of {algorithm_name} completed successfully.")
        
        run_result = RunResult(
            algorithm=algorithm_name,
            params=params,
            metrics={"bestFitnesses": result.bestFitnesses},
            snapshots={
                "initial": result.initialPopulation,
                "mid": result.midPopulation,
                "final": result.finalPopulation
            }
        )
        
        filename = DATA_DIR / f"{algorithm_name.lower()}_run_debug.txt"
        export_run_txt(run_result, filename)

        self.state.last_run_result = run_result
        self.state.history.append(run_result)

        self.resultReady.emit(run_result)
