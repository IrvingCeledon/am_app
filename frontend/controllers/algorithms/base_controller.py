import logging
import math

from frontend.native import core_module, kinematic_module
from frontend.config import BENCHMARKS

logger = logging.getLogger(__name__)

class BaseController:
    def _update_base_config(self, config_obj, p: dict):
        """Orchestrates the base configuration parsing."""
        config_obj.generations = p["generations"]
        config_obj.populationSize = p["population"]

        # Delegate problem setup based on the type
        problem_type = p.get("problem_type", "standard")
        if problem_type == "ik":
            self._setup_ik_problem(config_obj, p)
        else:
            self._setup_standard_problem(config_obj, p)

        # Stopping criteria (Shared across all problems)
        config_obj.use_target = p.get("use_target", False)
        config_obj.target_fitness = p.get("target_fitness", 0.0001)

        config_obj.use_stagnation = p.get("use_stagnation", False)
        config_obj.stagnation_patience = p.get("stagnation_patience", 20)

        if logger.isEnabledFor(logging.DEBUG):
            config_obj.verbosity = core_module.LogLevel.DEBUG
        else:
            config_obj.verbosity = core_module.LogLevel.INFO

    def _setup_standard_problem(self, config_obj, p: dict):
        """Configures domains and fitness for standard mathematical benchmarks."""
        dom = core_module.Domains()
        ranges = []

        x_i = p.get("x_i", 2)
        if x_i == 2:
            ranges.append(core_module.Range(p["x_min"], p["x_max"]))
            ranges.append(core_module.Range(p["y_min"], p["y_max"]))
        else:
            for _ in range(x_i):
                ranges.append(core_module.Range(p["x_min"], p["x_max"]))

        dom.ranges = ranges
        config_obj.domains = dom

        # Retrieve the C++ function pointer from the benchmarks dictionary
        config_obj.fitness = BENCHMARKS[p["fitness"]]["func"]
        config_obj.minimize = p.get("minimize", True)

    def _setup_ik_problem(self, config_obj, p: dict):
        """Configures domains and the functor for the 5-DOF Inverse Kinematics problem."""
        dom = core_module.Domains()

        # Strict Denavit-Hartenberg bounds for the modified Stanford arm
        dom.ranges = [
            core_module.Range(-math.pi, math.pi), # q1 (Revolute)
            core_module.Range(-math.pi, math.pi), # q2 (Revolute)
            core_module.Range(0.1, 1.0),          # q3 (Prismatic)
            core_module.Range(-math.pi, math.pi), # q4 (Revolute)
            core_module.Range(-math.pi, math.pi)  # q5 (Revolute)
        ]
        config_obj.domains = dom

        # Extract parameters from the IK UI Panel
        t_xyz = p.get("ik_target_xyz", [0.5, 0.5, 0.5])

        # Extract optional penalties
        t_quat = p.get("ik_target_quat", []) if p.get("ik_use_quat", False) else []
        p_posture = p.get("ik_prev_posture", []) if p.get("ik_use_prev", False) else []

        # Instantiate the C++ Functor evaluator
        # Using default penalty weights: 0.5 for joint motion, 10.0 for orientation
        ik_eval = kinematic_module.IKEvaluator(
            t_xyz,
            p_posture,
            t_quat,
            0.5,
            10.0
        )

        config_obj.fitness = ik_eval
        # The IK fitness function is f(q) = 1 / (1 + error), so we must MAXIMIZE it
        config_obj.minimize = False
