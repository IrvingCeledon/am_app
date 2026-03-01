from frontend.native import benchmarks_module


BENCHMARKS = {

    "f1": {
        "label": "f(x) = x³ + 4x² - 4x + 1",
        "func": benchmarks_module.cost_function_a,
        "x_min": -5.0, "x_max": 3.0,
        "y_min": -5.0, "y_max": 3.0,
        "target": -3.999  # ≈ -4.0
    },

    "f2": {
        "label": "f(x) = x⁴ + 5x³ + 4x² - 4x + 1",
        "func": benchmarks_module.cost_function_b,
        "x_min": -5.0, "x_max": 3.0,
        "y_min": -5.0, "y_max": 3.0,
        "target": -5.0
    },

    "f3": {
        "label": "Ackley with a = 20, b = 20, c = 2π",
        "func": benchmarks_module.cost_function_c,
        "x_min": -5.0, "x_max": 5.0,
        "y_min": -5.0, "y_max": 5.0,
        "target": -19.0
    },

    "ackley": {
        "label": "Ackley with a = 20, b = 0.2, c = 2π",
        "func": benchmarks_module.ackley,
        "x_min": -5.0, "x_max": 5.0,
        "y_min": -5.0, "y_max": 5.0,
        "target": 0.0001
    },

    "sphere": {
        "label": "Sphere",
        "func": benchmarks_module.sphere,
        "x_min": -5.12, "x_max": 5.12,
        "y_min": -5.12, "y_max": 5.12,
        "target": 0.0001
    },

    "eggholder": {
        "label": "Eggholder",
        "func": benchmarks_module.eggholder,
        "x_min": -512.0, "x_max": 512.0,
        "y_min": -512.0, "y_max": 512.0,
        "target": -959.64
    },

    "rosenbrock": {
        "label": "Rosenbrock",
        "func": benchmarks_module.rosenbrock,
        "x_min": -5.0, "x_max": 10.0,
        "y_min": -5.0, "y_max": 10.0,
        "target": 0.0001
    },

    "michalewicz": {
        "label": "Michalewicz",
        "func": benchmarks_module.michalewicz,
        "x_min": 0.0, "x_max": 3.14,
        "y_min": 0.0, "y_max": 3.14,
        "target": -1.801
    }
}
