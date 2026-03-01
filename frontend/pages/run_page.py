from PySide6.QtWidgets import (
    QWidget, 
    QSizePolicy, 
    QVBoxLayout, 
    QHBoxLayout, 
    QPushButton, 
    QLabel, 
    QComboBox
)

from PySide6.QtCore import Signal

from frontend.ui_components.graphs.graph_container import GraphContainer
from frontend.ui_components.controls.ga_controls import GAControls
from frontend.ui_components.controls.cga_controls import CGAControls

class RunPage(QWidget):
    runRequested = Signal(str, dict)
    backRequested = Signal()
    graph_changed = Signal(str)
    
    def __init__(self):
        super().__init__()
        
        self._last_result = None    # Just for draw 
        
        self._build_ui()
        self._connect_signals()

    def _build_ui(self):
        main_layout = QVBoxLayout(self)
        upper_controls = QHBoxLayout()
        
        self.controls_container = QHBoxLayout()
        self.controls_container.setStretch(0, 6)
        
        self.controls = GAControls()
        self.controls_container.addLayout(self.controls.build_layout())

        self.graph = GraphContainer()
        self.graph.setSizePolicy(
            QSizePolicy.Policy.Expanding,
            QSizePolicy.Policy.Expanding
        )

        upper_controls.addLayout( self._build_main_section(), 2 )
        upper_controls.addLayout( self.controls_container, 6 )

        main_layout.addLayout(upper_controls)
        main_layout.addWidget(self.graph)
        
    def _build_main_section(self):
        main_section = QVBoxLayout()

        # Algoritthms:
        algorithms = QHBoxLayout()
        self.algorithm_combo = QComboBox()
        self.algorithm_combo.addItems(["GA", "CGA"])
        
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
        self.run_btn = QPushButton("Run")
        self.run_btn.setObjectName("run_btn")
        self.return_btn = QPushButton("Return")
        self.return_btn.setObjectName("return_btn")
        
        main_section.addWidget(self.run_btn)
        main_section.addWidget(self.return_btn)   

        return main_section
        
    def _connect_signals(self):
        self.run_btn.clicked.connect(
            lambda: self.runRequested.emit(
                self.algorithm_combo.currentText(),
                self.controls.get_params()
            )
        )

        self.return_btn.clicked.connect(self.backRequested.emit)

        self.graphs_combo.currentTextChanged.connect(
            self._update_graph_by_selection
        )
        
        self.algorithm_combo.currentTextChanged.connect(
            self._on_algorithm_changed
        )

    def show_result(self, result):
        self._last_result = result
        self.graphs_combo.setCurrentIndex(0)
        self.graph.plot_fitness(result.metrics["bestFitnesses"])
        
    def _update_graph_by_selection(self, text):
        if not self._last_result:
            self.graphs_combo.setCurrentIndex(0)
            return

        snaps = self._last_result.snapshots

        if text == "none":
            self.graph.plot_fitness(self._last_result.metrics["bestFitnesses"])

        elif text == "all":
            self.graph.plot_populations_all([
                (snaps["initial"], "blue", "Initial"),
                (snaps["mid"], "orange", "Mid"),
                (snaps["final"], "green", "Final"),
            ])

        elif text == "initial":
            self.graph.plot_population(snaps["initial"], "blue", "Initial")

        elif text == "mid":
            self.graph.plot_population(snaps["mid"], "orange", "Mid")

        elif text == "final":
            self.graph.plot_population(snaps["final"], "green", "Final")
            
    def _on_algorithm_changed(self, algorithm: str):
        self._clear_controls()

        if algorithm == "GA":
            self.controls = GAControls()
        elif algorithm == "CGA":
            self.controls = CGAControls()
        else:
            return

        self.controls_container.addLayout( self.controls.build_layout() )
        
    def _clear_controls(self):
        while self.controls_container.count():
            item = self.controls_container.takeAt(0)

            if item.layout():
                self._delete_layout(item.layout())

    def _delete_layout(self, layout):
        while layout.count():
            child = layout.takeAt(0)
            if child.widget():
                child.widget().deleteLater()
            elif child.layout():
                self._delete_layout(child.layout())
