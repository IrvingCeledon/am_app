from PySide6.QtWidgets import (
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QComboBox,
    QCheckBox
)

import frontend.utils as ui
from frontend.utils import create_spin_box, create_double_spin_box
from frontend.config import BENCHMARKS


class Controls:        
    def __init__(self):
        super().__init__()
        
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
        self.x_i_spin = ui.create_spin_box(2, 10, 1, 2)
        x_i_layout.addWidget(QLabel("x_i:"))
        x_i_layout.addWidget(self.x_i_spin)
        
        # Signal connection
        self.x_i_spin.valueChanged.connect(self._on_x_i_changed)

        # Add sublayouts
        size_section.addLayout(generations)
        size_section.addLayout(population)
        size_section.addLayout(x_i_layout)
        
        return size_section
        
    def _on_x_i_changed(self, value):
        if value > 2:
            self.y_min.setEnabled(False)
            self.y_max.setEnabled(False)
        else:
            self.y_min.setEnabled(True)
            self.y_max.setEnabled(True)

    def _build_domains_section(self):
        domains_section = QVBoxLayout()

        # X range
        x_layout = QHBoxLayout()
        self.x_min = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        self.x_max = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        
        x_layout.addWidget(QLabel("X min:"))
        x_layout.addWidget(self.x_min)
        x_layout.addWidget(QLabel("X max:"))
        x_layout.addWidget(self.x_max)

        # Y range
        y_layout = QHBoxLayout()
        self.y_min = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        self.y_max = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        
        y_layout.addWidget(QLabel("Y min:"))
        y_layout.addWidget(self.y_min)
        y_layout.addWidget(QLabel("Y max:"))
        y_layout.addWidget(self.y_max)

        # Fitness function
        fitness_layout = QHBoxLayout()
        self.fitness_combo = QComboBox()
        
        for internal_key, data in BENCHMARKS.items():
            self.fitness_combo.addItem(data["label"], userData=internal_key)

        fitness_layout.addWidget(QLabel("Fitness:"))
        fitness_layout.addWidget(self.fitness_combo)

        # Add sublayouts
        domains_section.addLayout(x_layout)
        domains_section.addLayout(y_layout)
        domains_section.addLayout(fitness_layout)

        return domains_section
        
    def _build_miscellaneous_section(self):            
        misc_section = QVBoxLayout()

        # Target Fitness
        target_layout = QHBoxLayout()
        self.target_check = QCheckBox("Target Fitness:")
        self.target_spin = ui.create_double_spin_box(-1000.0, 1000.0, 0.00001, 0.0001, 5, False)
        
        # Signal to enable/disable the spinbox
        self.target_check.toggled.connect(self.target_spin.setEnabled)
        
        # Signal connection and forced update to synchronize values with fitness function
        # For lack of a better place, this connection will remain here.
        self.fitness_combo.currentIndexChanged.connect(self._on_fitness_changed)
        self._on_fitness_changed(0)
        
        target_layout.addWidget(self.target_check)
        target_layout.addWidget(self.target_spin)

        # Stagnation / Patience 
        stag_layout = QHBoxLayout()
        self.stag_check = QCheckBox("Stagnation:")
        self.stag_spin = ui.create_spin_box(5, 500, 5, 20, False)
        
        # Signal to enable/disable the spinbox
        self.stag_check.toggled.connect(self.stag_spin.setEnabled)

        stag_layout.addWidget(self.stag_check)
        stag_layout.addWidget(self.stag_spin)

        # Add sublayouts
        misc_section.addLayout(target_layout)
        misc_section.addLayout(stag_layout)

        return misc_section
        
    def _on_fitness_changed(self, index):
        fitness_id = self.fitness_combo.itemData(index)
        self.x_i_spin.setValue(2)
        
        if fitness_id in BENCHMARKS:
            data = BENCHMARKS[fitness_id]
            
            self.x_min.setValue(data["x_min"])
            self.x_max.setValue(data["x_max"])
            self.y_min.setValue(data["y_min"])
            self.y_max.setValue(data["y_max"])
            
            self.target_spin.setValue(data["target"])
