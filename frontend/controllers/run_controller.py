from PySide6.QtCore import QObject, Signal

import logging
logger = logging.getLogger(__name__)

from models import RunResult
from services import export_run_txt
from .algorithms import GAController

class RunController(QObject):
    resultReady = Signal(object) 

    def __init__(self, state):
        super().__init__()
        
        self.state = state
        self.algorithms = {
            "GA": GAController()
        }

    def run(self, algorithm_name: str, params: dict):
        logger.info(f"Running {algorithm_name}")
        
        if algorithm_name not in self.algorithms:
            raise ValueError(...)

        result = self.algorithms[algorithm_name].run_with_params(params)
        
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

        export_run_txt(run_result)

        self.state.last_run_result = run_result
        self.state.history.append(run_result)

        self.resultReady.emit(run_result)
