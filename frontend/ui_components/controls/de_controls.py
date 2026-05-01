from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QLabel
import frontend.utils as ui
from .controls import Controls

class DEControls(Controls):
    def __init__(self):
        super().__init__()

    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout(self._build_size_section())
        upper_controls.addLayout(self._build_domains_section())
        upper_controls.addLayout(self._build_de_params_section())
        upper_controls.addLayout(self._build_miscellaneous_section())

        return upper_controls

    def _build_de_params_section(self):
        params_section = QVBoxLayout()

        # Differential Weight (F)
        f_layout = QHBoxLayout()
        self.f_spin = ui.create_double_spin_box(0.0, 2.0, 0.1, 0.8)
        f_layout.addWidget(QLabel("Differential Weight (F):"))
        f_layout.addWidget(self.f_spin)

        # Crossover Probability (CR)
        cr_layout = QHBoxLayout()
        self.cr_spin = ui.create_double_spin_box(0.0, 1.0, 0.05, 0.9)
        cr_layout.addWidget(QLabel("Crossover Prob (CR):"))
        cr_layout.addWidget(self.cr_spin)

        params_section.addLayout(f_layout)
        params_section.addLayout(cr_layout)

        return params_section

    def get_params(self):
        # Get base parameters (size, domains, miscellaneous)
        params = super().get_params()

        # Add DE specific parameters
        params.update({
            "F": self.f_spin.value(),
            "CR": self.cr_spin.value()
        })

        return params
