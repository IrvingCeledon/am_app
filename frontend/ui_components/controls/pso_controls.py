from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QLabel
import frontend.utils as ui
from .controls import Controls

class PSOControls(Controls):
    def __init__(self):
        super().__init__()

    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout(self._build_size_section())
        upper_controls.addLayout(self._build_domains_section())
        upper_controls.addLayout(self._build_pso_params_section())
        upper_controls.addLayout(self._build_miscellaneous_section())

        return upper_controls

    def _build_pso_params_section(self):
        params_section = QVBoxLayout()

        # Inertia
        w_layout = QHBoxLayout()
        self.w_spin = ui.create_double_spin_box(0.0, 2.0, 0.05, 0.729)
        w_layout.addWidget(QLabel("Inertia (w):"))
        w_layout.addWidget(self.w_spin)

        # Cognitive
        c1_layout = QHBoxLayout()
        self.c1_spin = ui.create_double_spin_box(0.0, 4.0, 0.1, 1.496)
        c1_layout.addWidget(QLabel("Cognitive (c1):"))
        c1_layout.addWidget(self.c1_spin)

        # Social
        c2_layout = QHBoxLayout()
        self.c2_spin = ui.create_double_spin_box(0.0, 4.0, 0.1, 1.496)
        c2_layout.addWidget(QLabel("Social (c2):"))
        c2_layout.addWidget(self.c2_spin)

        params_section.addLayout(w_layout)
        params_section.addLayout(c1_layout)
        params_section.addLayout(c2_layout)

        return params_section

    def get_params(self):
        return {
            "generations": self.generations_spin.value(),
            "population": self.population_spin.value(),
            "x_i": self.x_i_spin.value(),
            "x_min": self.x_min.value(),
            "x_max": self.x_max.value(),
            "y_min": self.y_min.value(),
            "y_max": self.y_max.value(),
            "fitness": self.fitness_combo.currentData(),
            "minimize": self.opt_type_combo.currentData(),
            "use_target": self.target_check.isChecked(),
            "target_fitness": self.target_spin.value(),
            "use_stagnation": self.stag_check.isChecked(),
            "stagnation_patience": self.stag_spin.value(),
            "w": self.w_spin.value(),
            "c1": self.c1_spin.value(),
            "c2": self.c2_spin.value()
        }
