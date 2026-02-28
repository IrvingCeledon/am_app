from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QLabel, QComboBox
)

from PySide6.QtCore import Signal

from frontend.controllers.algorithms.ga_controller import GAController
from frontend.ui_components.graphs.graph_container import GraphContainer
from frontend.ui_components.controls.ga_controls import GAControls

class RunPage(QWidget):
    go_back = Signal()
    graph_changed = Signal(str)
    runRequested = Signal(dict)
    
    def __init__(self, state, stack):
        super().__init__()

        self.state = state
        self.stack = stack
        self.last_result = None
        
        self.ga_controller = GAController()
        
        self._build_ui()
        self._connect_signals()

    def _build_ui(self):
        main_layout = QVBoxLayout(self)
        upper_controls = QHBoxLayout()
        
        self.controls = GAControls()
        self.graph = GraphContainer()
        
        upper_controls.addLayout( self._build_main_section() )
        upper_controls.addWidget(self.controls)

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
        
        # Buttons:
        self.return_btn = QPushButton("Return")
        self.return_btn.setObjectName("return_btn")
        
        main_section.addWidget(self.controls.build_run_button())
        main_section.addWidget(self.return_btn)    
        
        return main_section

    def _update_graph_by_selection(self, text):
        if not self.last_result:
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
            self.graph.plot_population(
                self.last_result.initialPopulation,'blue', "Initial Population"
            )
        elif text == "mid":
            self.graph.plot_population(
                self.last_result.midPopulation, 'orange', "Mid Population"
            )
        elif text == "final":
            self.graph.plot_population(
                self.last_result.finalPopulation, 'green', "Final Population"
            )
            
    def _on_result_ready(self, result):
        self.last_result = result

        self.graphs_combo.setCurrentIndex(0)

        self.graph.plot_fitness(result.bestFitnesses)    
        
    def _connect_signals(self):
        self.return_btn.clicked.connect(
            lambda: self.stack.setCurrentIndex(0)
        )
        self.graphs_combo.currentTextChanged.connect(
            self._update_graph_by_selection
        )
        
        self.controls.runRequested.connect(
            self.ga_controller.run_with_params
        )

        self.ga_controller.resultReady.connect(
            self._on_result_ready
        )   
