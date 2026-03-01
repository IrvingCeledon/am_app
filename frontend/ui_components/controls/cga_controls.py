from PySide6.QtWidgets import (
    QVBoxLayout, 
    QHBoxLayout, 
    QLabel, 
    QComboBox, 
    QSpinBox, 
    QDoubleSpinBox
)

class CGAControls:
    def __init__(self):
        super().__init__()
        
    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout( self._build_size_section() )
        upper_controls.addLayout( self._build_domains_section() )
        upper_controls.addLayout( self._build_rates_section() )

        return upper_controls
        
    def _build_size_section(self):    
        size_section = QVBoxLayout()

        # Generations
        generations = QHBoxLayout()
        self.generations_spin = QSpinBox()
        self.generations_spin.setRange(50, 250)
        self.generations_spin.setSingleStep(10)
        self.generations_spin.setValue(60)
        generations.addWidget(QLabel("Generations:"))
        generations.addWidget(self.generations_spin)

        # Population
        population = QHBoxLayout()
        self.population_spin = QSpinBox()
        self.population_spin.setRange(10, 60)
        self.population_spin.setSingleStep(5)
        self.population_spin.setValue(50)
        population.addWidget(QLabel("Population:"))
        population.addWidget(self.population_spin)

        # Bits
        bits = QHBoxLayout()
        self.bits_spin = QSpinBox()
        self.bits_spin.setRange(4, 10)
        self.bits_spin.setSingleStep(4)
        self.bits_spin.setValue(4)
        bits.addWidget(QLabel("Bits:"))  
        bits.addWidget(self.bits_spin)

        # Add sublayouts
        size_section.addLayout(generations)
        size_section.addLayout(population)
        size_section.addLayout(bits)

        return size_section

    def _build_domains_section(self):
        range_section = QVBoxLayout()

        # X range
        x_layout = QHBoxLayout()
        
        self.x_min = QSpinBox()
        self.x_min.setRange(-5, 0)
        self.x_min.setValue(-5)
        self.x_max = QSpinBox()
        self.x_max.setRange(3, 5)
        self.x_max.setValue(3)
        
        x_layout.addWidget(QLabel("X min:"))
        x_layout.addWidget(self.x_min)
        x_layout.addWidget(QLabel("X max:"))
        x_layout.addWidget(self.x_max)

        # Y range
        y_layout = QHBoxLayout()
        
        self.y_min = QSpinBox()
        self.y_min.setRange(-5, 5)
        self.y_min.setValue(-5)
        self.y_max = QSpinBox()
        self.y_max.setRange(3, 5)
        self.y_max.setValue(3)
        y_layout.addWidget(QLabel("Y min:"))
        y_layout.addWidget(self.y_min)
        y_layout.addWidget(QLabel("Y max:"))
        y_layout.addWidget(self.y_max)

        # Fitness function
        fitness_layout = QHBoxLayout()
        self.fitness_combo = QComboBox()
        
        fitness_options = [
            ("f(x) = x³ + 4x² - 4x + 1", "f1"),
            ("f(x) = x⁴ + 5x³ + 4x² - 4x + 1", "f2"),
            ("f(x, y) = e − 20·exp{−20·√[(x² + y²)/2]} − exp{[cos(2πx) + cos(2πy)]/2}", "f3")
        ]
        
        for label, internal in fitness_options:
            self.fitness_combo.addItem(label, userData=internal)

        fitness_layout.addWidget(QLabel("Fitness:"))
        fitness_layout.addWidget(self.fitness_combo)

        # Add sublayouts
        range_section.addLayout(x_layout)
        range_section.addLayout(y_layout)
        range_section.addLayout(fitness_layout)

        return range_section

    def _build_rates_section(self):
        parameters_section = QVBoxLayout()

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
        parameters_section.addLayout(selection_layout)
        parameters_section.addLayout(crossover_layout)
        parameters_section.addLayout(mutation_layout)

        return parameters_section
            
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
