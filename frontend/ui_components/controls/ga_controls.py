from PySide6.QtWidgets import QHBoxLayout
from .controls import Controls

class GAControls(Controls):
    def __init__(self):
        super().__init__()
        
    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)
        
        fitness_options = [
            ("f(x) = x³ + 4x² - 4x + 1", "f1"),
            ("f(x) = x⁴ + 5x³ + 4x² - 4x + 1", "f2"),
            ("f(x, y) = e − 20·exp{−20·√[(x² + y²)/2]} − exp{[cos(2πx) + cos(2πy)]/2}", "f3")
        ]

        upper_controls.addLayout( self._build_size_section() )
        upper_controls.addLayout( self._build_domains_section(fitness_options) )
        upper_controls.addLayout( self._build_rates_section() )

        return upper_controls
            
    def get_params(self):
        return {
            "generations": self.generations_spin.value(),
            "population": self.population_spin.value(),
            "bits": self.bits_spin.value(),
            "x_min": self.x_min.value(),
            "x_max": self.x_max.value(),
            "y_min": self.y_min.value(),
            "y_max": self.y_max.value(),
            "fitness": self.fitness_combo.currentData(),
            "selection": self.selection_spin.value(),
            "crossover": self.crossover_spin.value(),
            "mutation": self.mutation_spin.value()
        }
