import sys
import random

from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget,
    QVBoxLayout, QHBoxLayout,
    QPushButton, QLabel, QComboBox, QSpinBox, QDoubleSpinBox
)

from PySide6.QtCore import Qt

from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from frontend.controllers.ga_controller import GAController
import mplcursors

class graph_container(FigureCanvas):
    def __init__(self):
        self.figure = Figure()
        super().__init__(self.figure)
        self.ax = self.figure.add_subplot(111)
        
        # Persistent cursor
        self.cursor = None
        
    def _attach_cursor(self, artist):
        if self.cursor:
            self.cursor.remove()  # avoids duplicate cursor

        self.cursor = mplcursors.cursor( artist, hover=mplcursors.HoverMode.Transient )


        @self.cursor.connect("add")
        def on_add(sel):
            x, y = sel.target
            sel.annotation.set_text(f"({x:.3f}, {y:.3f})")
            sel.annotation.get_bbox_patch().set(alpha=0.9)

    def plot_fitness(self, best_fitnesses):
        self.ax.clear()
        gens = range(1, len(best_fitnesses)+1)
        self.ax.plot(gens, best_fitnesses)          # line
        sc = self.ax.scatter(gens, best_fitnesses, s=15) # points
        self._attach_cursor(sc)
        self.ax.set_xlabel("Generation")
        self.ax.set_ylabel("Best Fitness")
        self.ax.set_title("Fitness over Generations")
        self.draw()

    def plot_population(self, population, color, title="Population"):
        # Graph individuals of pop (x, y)
        self.ax.clear()
        xs, ys = zip(*population)  # unpack tuples (x,y)
        sc = self.ax.scatter(xs, ys, c=color, alpha=0.4, s=20)
        self._attach_cursor(sc)
        self.ax.set_xlabel("X")
        self.ax.set_ylabel("Y")
        """
        self.ax.set_xlim(-8, 8)
        self.ax.set_ylim(-8, 8)
        self.ax.set_aspect('equal')
        """
        self.ax.set_title(title)
        self.draw()
        
    def plot_populations_all(self, populations):
        """
        populations: list of tuples -> (population, color, label)
        """
        self.ax.clear()

        artists = []

        for pop, color, label in populations:
            xs, ys = zip(*pop)
            sc = self.ax.scatter(xs, ys, c=color, alpha=0.4, s=20, label=label)
            artists.append(sc)

        self.ax.set_xlabel("X")
        self.ax.set_ylabel("Y")
        self.ax.set_title("All Populations")
        self.ax.legend()

        # cursor para TODOS los puntos
        if self.cursor:
            self.cursor.remove()

        self.cursor = mplcursors.cursor(artists, hover=mplcursors.HoverMode.Transient)

        @self.cursor.connect("add")
        def on_add(sel):
            x, y = sel.target
            sel.annotation.set_text(f"({x:.3f}, {y:.3f})")

        self.draw()

class RunPage(QWidget):
    def __init__(self, state, stack):
        super().__init__()

        self.state = state
        self.stack = stack

        self._build_ui()
        self._connect_signals()

    def _build_ui(self):
        main_layout = QVBoxLayout(self)
        upper_controls = QHBoxLayout()

        # Sections
        main_section = self._build_main_section()
        size_section = self._build_size_section()
        range_section = self._build_range_section()
        parameters_section = self._build_parameters_section()

        # Secondary layout
        upper_controls.addLayout(main_section)
        upper_controls.addLayout(size_section)
        upper_controls.addLayout(range_section)
        upper_controls.addLayout(parameters_section)
        
        # ---- Graph ----
        self.graph = graph_container()

        main_layout.addLayout(upper_controls)
        main_layout.addWidget(self.graph)
        
    def _build_main_section(self):
        main_section = QVBoxLayout()

        # Algoritthms:
        algorithms = QHBoxLayout()
        self.algorithm_combo = QComboBox()
        self.algorithm_combo.addItems(["GA"])
        algorithms.addWidget(QLabel("Algorithm:"))
        algorithms.addWidget(self.algorithm_combo)
        main_section.addLayout(algorithms)
        
        # Graphs:
        graphs = QHBoxLayout()
        self.graphs_combo = QComboBox()
        self.graphs_combo.addItems(["none", "all", "initial", "mid", "final"])
        graphs.addWidget(QLabel("See"))
        graphs.addWidget(self.graphs_combo)
        graphs.addWidget(QLabel("population"))
        main_section.addLayout(graphs)        
        
        # Controls:
        self.run_btn = QPushButton("Run")
        self.return_btn = QPushButton("Return")
        self.run_btn.setObjectName("run_btn")
        self.return_btn.setObjectName("return_btn")
        main_section.addWidget(self.run_btn)
        main_section.addWidget(self.return_btn)    
        
        return main_section
        
    def _build_size_section(self):    
        size_section = QVBoxLayout()

        # Generations
        generations = QHBoxLayout()
        generations.setSpacing(2)
        self.generations_spin = QSpinBox()
        self.generations_spin.setRange(50, 250)
        self.generations_spin.setSingleStep(10)
        self.generations_spin.setValue(60)
        generations.addWidget(QLabel("Generations:"))
        generations.addWidget(self.generations_spin)

        # Population
        population = QHBoxLayout()
        population.setSpacing(2)
        self.population_spin = QSpinBox()
        self.population_spin.setRange(10, 60)
        self.population_spin.setSingleStep(5)
        self.population_spin.setValue(50)
        population.addWidget(QLabel("Population:"))
        population.addWidget(self.population_spin)

        # Bits
        bits = QHBoxLayout()
        bits.setSpacing(2)
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

    def _build_range_section(self):
        range_section = QVBoxLayout()

        # X range
        x_layout = QHBoxLayout()
        x_layout.setSpacing(2)
        self.x_min = QSpinBox()
        self.x_min.setRange(-5, 0)
        self.x_min.setValue(-5)
        self.x_max = QSpinBox()
        self.x_max.setRange(3, 5)
        self.x_max.setValue(3)
        
        x_layout.addWidget(QLabel("X min:"))
        # self.x_min.setMaximumWidth(100)
        # self.x_min.setAlignment(Qt.AlignmentFlag.AlignLeft)
        x_layout.addWidget(self.x_min)
        x_layout.addWidget(QLabel("X max:"))
        # self.x_max.setMaximumWidth(100)
        # self.x_max.setAlignment(Qt.AlignmentFlag.AlignLeft)
        x_layout.addWidget(self.x_max)

        # Y range
        y_layout = QHBoxLayout()
        y_layout.setSpacing(2)
        self.y_min = QSpinBox()
        self.y_min.setRange(-5, 5)
        self.y_min.setValue(-5)
        self.y_max = QSpinBox()
        self.y_max.setRange(3, 5)
        self.y_max.setValue(3)
        y_layout.addWidget(QLabel("Y min:"))
        # self.y_min.setMaximumWidth(50)
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

    def _build_parameters_section(self):
        parameters_section = QVBoxLayout()

        # Selection
        selection_layout = QHBoxLayout()
        selection_layout.setSpacing(2)
        self.selection_spin = QDoubleSpinBox()
        self.selection_spin.setRange(0.1, 1.0)  # 10% to 100%
        self.selection_spin.setSingleStep(0.05) # single step of 5%
        self.selection_spin.setValue(0.5)       # default of 50%
        selection_layout.addWidget(QLabel("Selection:"))
        selection_layout.addWidget(self.selection_spin)

        # Crossover
        crossover_layout = QHBoxLayout()
        crossover_layout.setSpacing(2)
        self.crossover_spin = QDoubleSpinBox()
        self.crossover_spin.setRange(0.0, 1.0)  # 0% to 100%
        self.crossover_spin.setSingleStep(0.05) # single step of 5%
        self.crossover_spin.setValue(0.7)       # default of 70%
        crossover_layout.addWidget(QLabel("Crossover:"))
        crossover_layout.addWidget(self.crossover_spin)

        # Mutation
        mutation_layout = QHBoxLayout()
        mutation_layout.setSpacing(2)
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
        
    # Example simulation
    def run_algorithm(self):
        gens = self.generations_spin.value()

        data = [100/(i+1) + random.random()*5 for i in range(gens)]

        self.graph.plot_data(data)

    def _connect_signals(self):
        self.return_btn.clicked.connect(lambda: self.stack.setCurrentIndex(0))
        self.graphs_combo.currentTextChanged.connect(self.update_graph_by_selection)
        self.run_btn.clicked.connect(self.run_ga)

    def update_graph_by_selection(self, text):
        # Updates depending on graphs_combo value.
        if not hasattr(self, "last_result") or self.last_result is None:
            self.graphs_combo.setCurrentIndex(0)
            return  # There is no data

        if text == "none":
            self.graph.plot_fitness(self.last_result.bestFitnesses)
        elif text == "all":
            self.graph.plot_populations_all([
                (self.last_result.initialPopulation, "blue", "Initial"),
                (self.last_result.midPopulation, "orange", "Mid"),
                (self.last_result.finalPopulation, "green", "Final"),
            ])
        elif text == "initial":
            self.graph.plot_population(self.last_result.initialPopulation
                                      ,'blue', "Initial Population")
        elif text == "mid":
            self.graph.plot_population(self.last_result.midPopulation
                                      ,'orange', "Mid Population")
        elif text == "final":
            self.graph.plot_population(self.last_result.finalPopulation
                                      ,'green', "Final Population")

    def run_ga(self):
        # 1. Create GAController if not exist
        if not hasattr(self, "ga_controller"):
            self.ga_controller = GAController()
            
        self.graphs_combo.setCurrentIndex(0)

        # 2. Pass widgets to constructor
        self.ga_controller.set_parameters_from_widgets(
            generations_spin=self.generations_spin,
            population_spin=self.population_spin,
            bits_spin=self.bits_spin,
            x_min_spin=self.x_min,
            x_max_spin=self.x_max,
            y_min_spin=self.y_min,
            y_max_spin=self.y_max,
            fitness_combo=self.fitness_combo,
            selection_spin=self.selection_spin,
            crossover_spin=self.crossover_spin,
            mutation_spin=self.mutation_spin
        )

        # 3. Execute GA
        self.last_result = self.ga_controller.run()  # Return RunResult
        self._export_run_to_txt()

        # 4. Show fitness graph as default
        self.graph.plot_fitness(self.last_result.bestFitnesses)
        
    def _export_run_to_txt(self, filename="data/ga_debug.txt"):
        r = self.last_result

        with open(filename, "w") as f:
            f.write("===== GA DEBUG REPORT =====\n\n")

            f.write("Parameters:\n")
            f.write(f"Generations: {self.generations_spin.value()}\n")
            f.write(f"Population: {self.population_spin.value()}\n")
            f.write(f"Bits: {self.bits_spin.value()}\n")
            f.write(f"Selection: {self.selection_spin.value()}\n")
            f.write(f"Crossover: {self.crossover_spin.value()}\n")
            f.write(f"Mutation: {self.mutation_spin.value()}\n\n")

            f.write("Best fitness per generation:\n")
            for i, fit in enumerate(r.bestFitnesses, 1):
                f.write(f"{i}\t{fit}\n")

            def dump_population(name, pop):
                f.write(f"\n{name} population:\n")
                for x, y in pop:
                    f.write(f"{x}\t{y}\n")

            dump_population("Initial", r.initialPopulation)
            dump_population("Mid", r.midPopulation)
            dump_population("Final", r.finalPopulation)

