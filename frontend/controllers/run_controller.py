from PySide6.QtCore import QObject, Signal
import math
import json

import logging

from frontend.paths import DATA_DIR
from models import RunResult
from services import export_run_txt
from frontend.controllers.algorithms import *
from frontend.native import kinematic_module


logger = logging.getLogger(__name__)


class RunController(QObject):
    resultReady = Signal(object) 

    def __init__(self, state):
        super().__init__()
        
        self.state = state
        self.algorithms = {
            "GA": GAController(),
            "CGA": CGAController(),
            "ABC": ABCController(),
            "PSO": PSOController(),
            "DE": DEController(),
            "BFOA": BFOAController()
        }

    def run(self, algorithm_name: str, params: dict):
        logger.info(f"Running {algorithm_name}")
        formatted_params = json.dumps(params, indent=4)
        logger.debug(f"Received parameters:\n{formatted_params}")
        
        if algorithm_name not in self.algorithms:
            raise ValueError(...)

        result = self.algorithms[algorithm_name].run_with_params(params)
        
        for entry in result.logs:
            # Cast of C++ enum based on Python emulation, int (10, 20, 30, 40)
            log_level_int = int(entry.level) 
            logger.log(log_level_int, f"[C++ {algorithm_name}] {entry.message}")

        if params.get("problem_type") == "ik":
            # Best individual from the final population
            best_genome = result.finalPopulation[0]

            # Forward kinematics evaluation
            transform_matrix = kinematic_module.get_forward_kinematics(best_genome)
            quat = kinematic_module.get_quaternion(transform_matrix)

            # 1. Extract physical XYZ position from the 4x4 matrix
            final_x = transform_matrix[0][3]
            final_y = transform_matrix[1][3]
            final_z = transform_matrix[2][3]

            # 2. Calculate spatial Euclidean error
            target_xyz = params.get("ik_target_xyz", [0.0, 0.0, 0.0])
            error_spatial = math.sqrt(
                (final_x - target_xyz[0])**2 +
                (final_y - target_xyz[1])**2 +
                (final_z - target_xyz[2])**2
            )

            # Log 1: Joint Configuration (Genome)
            logger.info(f"[IK Result] Joint Configuration: "
                        f"q1={best_genome[0]:.4f} rad, q2={best_genome[1]:.4f} rad, "
                        f"q3={best_genome[2]:.4f} m, q4={best_genome[3]:.4f} rad, "
                        f"q5={best_genome[4]:.4f} rad")

            # Log 2: Spatial Position and Error
            logger.info(f"[IK Result] Reached XYZ: "
                        f"({final_x:.4f}, {final_y:.4f}, {final_z:.4f}) | "
                        f"Spatial Error: {error_spatial:.6f} meters")

            # Log 3: Orientation (Quaternion)
            logger.info(f"[IK Result] Final Quaternion: "
                        f"w={quat[0]:.4f}, x={quat[1]:.4f}, y={quat[2]:.4f}, z={quat[3]:.4f}")
            
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
