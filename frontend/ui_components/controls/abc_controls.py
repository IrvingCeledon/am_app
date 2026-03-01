from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QLabel
from frontend.utils import create_spin_box
from .controls import Controls

class ABCControls(Controls):        
    def __init__(self):
        super().__init__()
        
    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout(self._build_size_section())
        upper_controls.addLayout(self._build_domains_section())
        upper_controls.addLayout(self._build_abc_miscellaneous_section())

        return upper_controls

    def _build_abc_miscellaneous_section(self):           
        abc_miscellaneous_section = self._build_miscellaneous_section()
        
        # Limit (Paciencia de las fuentes de comida)
        limit_layout = QHBoxLayout()
        self.limit_spin = create_spin_box(10, 1000, 10, 100)
        limit_layout.addWidget(QLabel("Limit (Abandonment):"))
        limit_layout.addWidget(self.limit_spin)

        abc_miscellaneous_section.addLayout(limit_layout)

        return abc_miscellaneous_section
            
    def get_params(self):
        return {
            "generations": self.generations_spin.value(),
            "population": self.population_spin.value(), # En ABC equivale a SN (Food Sources)
            "x_i": self.x_i_spin.value(),
            "x_min": self.x_min.value(),
            "x_max": self.x_max.value(),
            "y_min": self.y_min.value(),
            "y_max": self.y_max.value(),
            "fitness": self.fitness_combo.currentData(),
            "use_target": self.target_check.isChecked(),
            "target_fitness": self.target_spin.value(),
            "use_stagnation": self.stag_check.isChecked(),
            "stagnation_patience": self.stag_spin.value(),
            "limit": self.limit_spin.value()
        }
