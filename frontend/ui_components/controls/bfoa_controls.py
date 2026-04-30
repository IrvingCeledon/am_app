from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QLabel
import frontend.utils as ui
from .controls import Controls

class BFOAControls(Controls):
    def __init__(self):
        super().__init__()
        # Ocultamos/Deshabilitamos el control genérico de 'generations' de BaseControls
        # ya que BFOA usa ciclos anidados.
        self.generations_spin.setEnabled(False)

    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout(self._build_size_section())
        upper_controls.addLayout(self._build_domains_section())
        upper_controls.addLayout(self._build_bfoa_cycles_section())
        upper_controls.addLayout(self._build_bfoa_params_section())
        upper_controls.addLayout(self._build_miscellaneous_section())

        return upper_controls

    def _build_bfoa_cycles_section(self):
        cycles_section = QVBoxLayout()

        # Chemotactic Steps (j)
        chemo_layout = QHBoxLayout()
        self.chemo_spin = ui.create_spin_box(1, 1000, 10, 20)
        chemo_layout.addWidget(QLabel("Chemo Steps (j):"))
        chemo_layout.addWidget(self.chemo_spin)

        # Reproduction Steps (k)
        repro_layout = QHBoxLayout()
        self.repro_spin = ui.create_spin_box(1, 100, 1, 4)
        repro_layout.addWidget(QLabel("Repro Steps (k):"))
        repro_layout.addWidget(self.repro_spin)

        # Elimination-Dispersal Steps (l)
        elim_layout = QHBoxLayout()
        self.elim_spin = ui.create_spin_box(1, 100, 1, 2)
        elim_layout.addWidget(QLabel("Elim-Disp Steps (l):"))
        elim_layout.addWidget(self.elim_spin)

        cycles_section.addLayout(chemo_layout)
        cycles_section.addLayout(repro_layout)
        cycles_section.addLayout(elim_layout)

        return cycles_section

    def _build_bfoa_params_section(self):
        params_section = QVBoxLayout()

        # Step Size
        step_layout = QHBoxLayout()
        self.step_spin = ui.create_double_spin_box(0.01, 10.0, 0.1, 0.1)
        step_layout.addWidget(QLabel("Step Size:"))
        step_layout.addWidget(self.step_spin)

        # Run Length
        length_layout = QHBoxLayout()
        self.length_spin = ui.create_spin_box(1, 50, 1, 4)
        length_layout.addWidget(QLabel("Run Length:"))
        length_layout.addWidget(self.length_spin)

        # Elimination Rate
        elim_rate_layout = QHBoxLayout()
        self.elim_rate_spin = ui.create_double_spin_box(0.0, 1.0, 0.05, 0.25)
        elim_rate_layout.addWidget(QLabel("Elimination Rate:"))
        elim_rate_layout.addWidget(self.elim_rate_spin)

        params_section.addLayout(step_layout)
        params_section.addLayout(length_layout)
        params_section.addLayout(elim_rate_layout)

        return params_section

    def get_params(self):
        # Tomamos los parámetros base
        params = super().get_params() if hasattr(super(), 'get_params') else {
            "generations": 0, # Ignorado en backend
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
        }

        # Agregamos los parámetros específicos de BFOA
        params.update({
            "chemotactic_steps": self.chemo_spin.value(),
            "reproduction_steps": self.repro_spin.value(),
            "elimination_dispersal_steps": self.elim_spin.value(),
            "step_size": self.step_spin.value(),
            "run_length": self.length_spin.value(),
            "elimination_rate": self.elim_rate_spin.value(),
            # Swarming defaults (podrías añadir spins para esto si quieres editarlos visualmente)
            "d_attract": 0.1,
            "w_attract": 0.2,
            "h_repel": 0.1,
            "w_repel": 10.0
        })
        return params
