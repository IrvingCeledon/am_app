from PySide6.QtWidgets import (
    QVBoxLayout, 
    QHBoxLayout, 
    QLabel, 
    QComboBox, 
    QSpinBox, 
    QDoubleSpinBox
)
from PySide6.QtCore import Signal

from .controls import Controls

class CGAControls(Controls):
    def __init__(self):
        super().__init__()
        
    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)
        
        fitness_options = [
            ("Ackley:", "ackley"),
            ("Sphere", "sphere")
        ]

        upper_controls.addLayout( self._build_size_section() )
        upper_controls.addLayout( self._build_cga_domains_section(fitness_options) )
        upper_controls.addLayout( self._build_rates_section() )
        upper_controls.addLayout( self._build_cga_section() )
        
        self.bits_label.hide()
        self.bits_spin.hide()
        
        self.x_i_spin.valueChanged.connect(self._on_x_i_changed)

        return upper_controls
        
    def _build_cga_domains_section(self, fitness_options):
        range_section = QVBoxLayout()

        # X range
        x_layout = QHBoxLayout()
        
        self.cga_x_min = QDoubleSpinBox()
        self.cga_x_min.setRange(-5.12, 0)
        self.cga_x_min.setValue(-5.12)
        self.cga_x_max = QDoubleSpinBox()
        self.cga_x_max.setRange(3.0, 5.12)
        self.cga_x_max.setValue(5.12)
        
        x_layout.addWidget(QLabel("X min:"))
        x_layout.addWidget(self.cga_x_min)
        x_layout.addWidget(QLabel("X max:"))
        x_layout.addWidget(self.cga_x_max)

        # Y range
        y_layout = QHBoxLayout()
        
        self.cga_y_min = QSpinBox()
        self.cga_y_min.setRange(-5, 5)
        self.cga_y_min.setValue(-5)
        self.cga_y_max = QSpinBox()
        self.cga_y_max.setRange(3, 5)
        self.cga_y_max.setValue(3)
        y_layout.addWidget(QLabel("Y min:"))
        y_layout.addWidget(self.cga_y_min)
        y_layout.addWidget(QLabel("Y max:"))
        y_layout.addWidget(self.cga_y_max)

        # Fitness function
        fitness_layout = QHBoxLayout()
        self.fitness_combo = QComboBox()
        
        for label, internal in fitness_options:
            self.fitness_combo.addItem(label, userData=internal)

        fitness_layout.addWidget(QLabel("Fitness:"))
        fitness_layout.addWidget(self.fitness_combo)

        # Add sublayouts
        range_section.addLayout(x_layout)
        range_section.addLayout(y_layout)
        range_section.addLayout(fitness_layout)

        return range_section

    def _build_cga_section(self):           
        cga_section = QVBoxLayout()
        
        # Number of variables
        x_i_layout = QHBoxLayout()
        self.x_i_spin = QSpinBox()
        self.x_i_spin.setRange(2, 10)
        self.x_i_spin.setSingleStep(1)
        self.x_i_spin.setValue(2)
        x_i_layout.addWidget(QLabel("x_i:"))
        x_i_layout.addWidget(self.x_i_spin)
        
        # Mutation scale
        scale_layout = QHBoxLayout()
        self.mutation_scale_spin = QDoubleSpinBox()
        self.mutation_scale_spin.setRange(0.0, 1.0)  # 0% to 100%
        self.mutation_scale_spin.setSingleStep(0.05) # single step of 5%
        self.mutation_scale_spin.setValue(0.05)       # default of 70%
        scale_layout.addWidget(QLabel("Mutation Scale:"))
        scale_layout.addWidget(self.mutation_scale_spin)

        # Stopping criteria
        stopping_layout = QHBoxLayout()
        self.stopping_combo = QComboBox()
        self.stopping_combo.addItems(["Fixed Generations"])
        
        stopping_layout.addWidget(QLabel("Stopping criteria:"))
        stopping_layout.addWidget(self.stopping_combo)

        # Add sublayouts
        cga_section.addLayout(x_i_layout)
        cga_section.addLayout(scale_layout)
        cga_section.addLayout(stopping_layout)

        return cga_section
            
    def _on_x_i_changed(self, value):
        if value > 2:
            self.cga_y_min.setEnabled(False)
            self.cga_y_max.setEnabled(False)
        else:
            self.cga_y_min.setEnabled(True)
            self.cga_y_max.setEnabled(True)
            
    def get_params(self):
        return {
            "generations": self.generations_spin.value(),
            "population": self.population_spin.value(),
            "x_min": self.cga_x_min.value(),
            "x_max": self.cga_x_max.value(),
            "y_min": self.cga_y_min.value(),
            "y_max": self.cga_y_max.value(),
            "fitness": self.fitness_combo.currentData(),
            "selection": self.selection_spin.value(),
            "crossover": self.crossover_spin.value(),
            "mutation": self.mutation_spin.value(),
            "x_i": self.x_i_spin.value(),
            "mutation_scale": self.mutation_scale_spin.value(),
            "stopping": self.stopping_combo.currentText()
        }
