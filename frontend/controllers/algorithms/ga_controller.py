from PySide6.QtCore import QObject, Signal

from frontend.native import ga_module
from models import RunResult
from services import export_run_txt

class GAController(QObject):
    resultReady = Signal(object)
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        self.config = ga_module.GAConfig()
        self.ga = ga_module.GA(self.config)
        
    def _update_config(self, p: dict):
        fitness_map = {
            "f1": ga_module.f1,
            "f2": ga_module.f2,
            "f3": ga_module.f3
        }

        self.config.generations = p["generations"]
        self.config.populationSize = p["population"]
        self.config.bits = p["bits"]

        self.config.domains.x.min = p["x_min"]
        self.config.domains.x.max = p["x_max"]
        self.config.domains.y.min = p["y_min"]
        self.config.domains.y.max = p["y_max"]

        self.config.fitness = fitness_map[p["fitness"]]

        self.config.rates.selection = p["selection"]
        self.config.rates.crossover = p["crossover"]
        self.config.rates.mutation = p["mutation"]
        
    def run_with_params(self, params: dict):
        self._update_config(params)

        self.ga = ga_module.GA(self.config)
        
        result = self.ga.run()
        
        run_result = RunResult(
            algorithm="GA",
            params=params,
            metrics={"bestFitnesses": result.bestFitnesses},
            snapshots={
                "initial": result.initialPopulation,
                "mid": result.midPopulation,
                "final": result.finalPopulation
            }
        )

        export_run_txt(run_result)
        
        self.resultReady.emit(result)
