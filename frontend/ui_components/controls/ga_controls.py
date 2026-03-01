from PySide6.QtWidgets import (
    QVBoxLayout, 
    QHBoxLayout, 
    QLabel, 
    QSpinBox,
    QDoubleSpinBox
)

from .controls import Controls

class GAControls(Controls):
    def __init__(self):
        super().__init__()
        
    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout( self._build_size_section() )
        upper_controls.addLayout( self._build_domains_section() )
        upper_controls.addLayout( self._build_ga_rates_section() )
        upper_controls.addLayout( self._build_ga_miscellaneous_section() )

        return upper_controls
        
    def _build_ga_rates_section(self):
        rates_section = QVBoxLayout()

        # Selection
        selection_layout = QHBoxLayout()
        self.selection_spin = QDoubleSpinBox()
        self.selection_spin.setRange(0.1, 1.0)  # 10% to 100%
        self.selection_spin.setSingleStep(0.05) # single step of 5%
        self.selection_spin.setValue(0.5)       # default of 50%
        selection_layout.addWidget(QLabel("Selection:"))
        selection_layout.addWidget(self.selection_spin)

        # Crossover
        crossover_layout = QHBoxLayout()
        self.crossover_spin = QDoubleSpinBox()
        self.crossover_spin.setRange(0.0, 1.0)  # 0% to 100%
        self.crossover_spin.setSingleStep(0.05) # single step of 5%
        self.crossover_spin.setValue(0.7)       # default of 70%
        crossover_layout.addWidget(QLabel("Crossover:"))
        crossover_layout.addWidget(self.crossover_spin)

        # Mutation
        mutation_layout = QHBoxLayout()
        self.mutation_spin = QDoubleSpinBox()
        self.mutation_spin.setRange(0.0, 0.1)  # 0% to 10%
        self.mutation_spin.setSingleStep(0.01) # single step of 1%
        self.mutation_spin.setValue(0.01)       # default of 1%
        mutation_layout.addWidget(QLabel("Mutation:"))
        mutation_layout.addWidget(self.mutation_spin)

        # Add sublayouts
        rates_section.addLayout(selection_layout)
        rates_section.addLayout(crossover_layout)
        rates_section.addLayout(mutation_layout)

        return rates_section
        
    def _build_ga_miscellaneous_section(self):
        ga_miscellaneous_section = self._build_miscellaneous_section()

        # Bits
        bits = QHBoxLayout()
        self.bits_spin = QSpinBox()
        self.bits_spin.setRange(2, 32)
        self.bits_spin.setSingleStep(2)
        self.bits_spin.setValue(4)
        self.bits_label = QLabel("Bits:")
        bits.addWidget(self.bits_label)  
        bits.addWidget(self.bits_spin)

        # Add ga sublayout
        ga_miscellaneous_section.addLayout(bits)

        return ga_miscellaneous_section
            
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
            "selection": self.selection_spin.value(),
            "crossover": self.crossover_spin.value(),
            "mutation": self.mutation_spin.value(),
            "use_target": self.target_check.isChecked(),
            "target_fitness": self.target_spin.value(),
            "use_stagnation": self.stag_check.isChecked(),
            "stagnation_patience": self.stag_spin.value(),
            "bits": self.bits_spin.value(),
        }
