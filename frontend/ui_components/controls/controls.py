from PySide6.QtWidgets import (
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QComboBox,
    QCheckBox,
    QStackedWidget
)

import frontend.utils as ui
from frontend.config import BENCHMARKS
from .problem_panels import StandardProblemPanel, IKProblemPanel


class Controls:
    def __init__(self):
        super().__init__()
        # Inicializamos los paneles de los problemas
        self.panels = {
            "standard": StandardProblemPanel(),
            "ik": IKProblemPanel()
        }

    def _build_size_section(self):
        size_section = QVBoxLayout()

        # Generations
        generations = QHBoxLayout()
        self.generations_spin = ui.create_spin_box(20, 9999, 10, 60)
        generations.addWidget(QLabel("Generations:"))
        generations.addWidget(self.generations_spin)

        # Population
        population = QHBoxLayout()
        self.population_spin = ui.create_spin_box(10, 300, 5, 50)
        population.addWidget(QLabel("Population:"))
        population.addWidget(self.population_spin)

        # Number of variables
        x_i_layout = QHBoxLayout()
        self.x_i_spin = ui.create_spin_box(1, 10, 1, 2)
        x_i_layout.addWidget(QLabel("x_i:"))
        x_i_layout.addWidget(self.x_i_spin)

        # Signal connection (Delegado al panel estándar)
        self.x_i_spin.valueChanged.connect(self._on_x_i_changed)

        size_section.addLayout(generations)
        size_section.addLayout(population)
        size_section.addLayout(x_i_layout)

        return size_section

    def _on_x_i_changed(self, value):
        # Solo el panel estándar necesita saber esto para habilitar/deshabilitar Y
        if hasattr(self.panels["standard"], "on_dims_changed"):
            self.panels["standard"].on_dims_changed(value)

    def _build_domains_section(self):
        domains_section = QVBoxLayout()

        # Fitness function selector
        fitness_layout = QHBoxLayout()
        self.fitness_combo = QComboBox()

        for internal_key, data in BENCHMARKS.items():
            self.fitness_combo.addItem(data["label"], userData=internal_key)

        fitness_layout.addWidget(QLabel("Fitness / Problem:"))
        fitness_layout.addWidget(self.fitness_combo)

        domains_section.addLayout(fitness_layout)

        # StackedWidget para los paneles de problemas
        self.problem_stack = QStackedWidget()
        self.problem_stack.addWidget(self.panels["standard"])
        self.problem_stack.addWidget(self.panels["ik"])

        domains_section.addWidget(self.problem_stack)

        return domains_section

    def _build_miscellaneous_section(self):
        misc_section = QVBoxLayout()

        # Target Fitness
        target_layout = QHBoxLayout()
        self.target_check = QCheckBox("Target Fitness:")
        self.target_spin = ui.create_double_spin_box(-1000.0, 1000.0, 0.00001, 0.0001, 5, False)

        self.target_check.toggled.connect(self.target_spin.setEnabled)

        target_layout.addWidget(self.target_check)
        target_layout.addWidget(self.target_spin)

        # Stagnation / Patience
        stag_layout = QHBoxLayout()
        self.stag_check = QCheckBox("Stagnation:")
        self.stag_spin = ui.create_spin_box(5, 500, 5, 20, False)

        self.stag_check.toggled.connect(self.stag_spin.setEnabled)

        stag_layout.addWidget(self.stag_check)
        stag_layout.addWidget(self.stag_spin)

        misc_section.addLayout(target_layout)
        misc_section.addLayout(stag_layout)

        # Conexión principal para orquestar cambios (Se hace al final para tener la UI lista)
        self.fitness_combo.currentIndexChanged.connect(self._on_fitness_changed)
        self._on_fitness_changed(0)

        return misc_section

    def _on_fitness_changed(self, index):
        fitness_id = self.fitness_combo.itemData(index)

        if fitness_id in BENCHMARKS:
            data = BENCHMARKS[fitness_id]
            prob_type = data.get("type", "standard")

            # 1. Cambiar el panel activo
            active_panel = self.panels.get(prob_type, self.panels["standard"])
            self.problem_stack.setCurrentWidget(active_panel)

            # 2. Actualizar la UI del panel hijo
            active_panel.update_ui(data)

            # 3. Forzar configuraciones base según el tipo
            self.target_spin.setValue(data["target"])

            if prob_type == "ik":
                self.x_i_spin.setValue(int(data["dims"]))
                self.x_i_spin.setEnabled(False)
                self.target_check.setChecked(True)
            else:
                if data["dims"] == "N":
                    self.x_i_spin.setEnabled(True)
                    self.x_i_spin.setValue(2)
                else:
                    self.x_i_spin.setValue(int(data["dims"]))
                    self.x_i_spin.setEnabled(False)

    def get_params(self):
        active_panel = self.problem_stack.currentWidget()

        params = {
            "generations": self.generations_spin.value(),
            "population": self.population_spin.value(),
            "x_i": self.x_i_spin.value(),
            "fitness": self.fitness_combo.currentData(),
            "use_target": self.target_check.isChecked(),
            "target_fitness": self.target_spin.value(),
            "use_stagnation": self.stag_check.isChecked(),
            "stagnation_patience": self.stag_spin.value(),
        }

        # Extraemos los datos dinámicos (Standard o IK)
        params.update(active_panel.get_params())

        return params
