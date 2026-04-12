from PySide6.QtWidgets import (
    QWidget,
    QStackedWidget,
    QSizePolicy,
    QVBoxLayout,
    QHBoxLayout,
    QPushButton,
    QLabel,
    QComboBox
)

from PySide6.QtCore import Qt, Signal

from frontend.ui_components.graphs.graph_container import GraphContainer
from frontend.ui_components.controls import *


class RunPage(QWidget):
    runRequested = Signal(str, dict)
    backRequested = Signal()
    graph_changed = Signal(str)
    
    GRAPH_OPTIONS = [
        ("none", "none"),
        ("all", "all"),
        ("initial", "initial"),
        ("mid", "mid"),
        ("final", "final")
    ]

    def __init__(self):
        super().__init__()

        self._last_result = None    # Just for draw
        
        self.algorithms_registry = {
            "GA": GAControls(),
            "CGA": CGAControls(),
            "ABC": ABCControls(),
            "PSO": PSOControls(),
            "DE": DEControls()
        }
        
        self.controls_widgets = {}

        self._build_ui()
        self._connect_signals()

    def _build_ui(self):
        main_layout = QVBoxLayout(self)
        upper_controls = QHBoxLayout()

        self.controls_stack = QStackedWidget()
        self.controls_stack.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Minimum)

        for name, controls_instance in self.algorithms_registry.items():
            widget = QWidget()
            layout = controls_instance.build_layout()
            layout.setAlignment(Qt.AlignmentFlag.AlignTop)
            widget.setLayout(layout)
            
            self.controls_stack.addWidget(widget)
            self.controls_widgets[name] = widget
            
        aux_control = list(self.algorithms_registry.keys())[0]
        self.controls = self.algorithms_registry[aux_control]

        self.graph = GraphContainer()
        self.graph.setSizePolicy(
            QSizePolicy.Policy.Expanding,
            QSizePolicy.Policy.Expanding
        )

        upper_controls.addLayout(self._build_main_section(), 2)
        upper_controls.addWidget(self.controls_stack, 6)

        main_layout.addLayout(upper_controls, 0)
        main_layout.addWidget(self.graph, 1)

    def _build_main_section(self):
        main_section = QVBoxLayout()
        main_section.setAlignment(Qt.AlignmentFlag.AlignTop)

        # Algorithms:
        algorithms = QHBoxLayout()
        self.algorithm_combo = QComboBox()
        self.algorithm_combo.addItems(self.algorithms_registry.keys())

        algorithms.addWidget(QLabel("Algorithm:"))
        algorithms.addWidget(self.algorithm_combo)
        main_section.addLayout(algorithms)

        # Graphs:
        graphs = QHBoxLayout()
        self.graphs_combo = QComboBox()

        for label, internal in self.GRAPH_OPTIONS:
            self.graphs_combo.addItem(label, userData=internal)

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

        self.graphs_combo.currentIndexChanged.connect(
            self._update_graph_by_selection
        )

        self.algorithm_combo.currentTextChanged.connect(
            self._on_algorithm_changed
        )

    def _update_graph_by_selection(self, index):
        if not self._last_result or index == -1:
            self.graphs_combo.setCurrentIndex(0)
            return

        internal_data = self.graphs_combo.itemData(index)
        snaps = self._last_result.snapshots
        params = self._last_result.params # <--- Extraemos los parámetros

        if internal_data == "none":
            self.graph.plot_fitness(self._last_result.metrics["bestFitnesses"])

        elif internal_data == "all":
            plots = [(snaps["initial"], "blue", "Initial")]
            if len(snaps["mid"]) > 0:
                plots.append((snaps["mid"], "orange", "Mid"))
            plots.append((snaps["final"], "green", "Final"))

            # Pasamos params
            self.graph.plot_populations_all(plots, params)

        elif internal_data == "initial":
            # Pasamos params
            self.graph.plot_population(snaps["initial"], "blue", "Initial", params)

        elif internal_data == "mid":
            self.graph.plot_population(snaps["mid"], "orange", "Mid", params)

        elif internal_data == "final":
            self.graph.plot_population(snaps["final"], "green", "Final", params)

    def _on_algorithm_changed(self, algorithm: str):
        if algorithm in self.algorithms_registry: 
            self.controls_stack.setCurrentWidget(self.controls_widgets[algorithm])
            self.controls = self.algorithms_registry[algorithm]
            
    def show_result(self, result):
        self._last_result = result

        has_mid_population = len(result.snapshots["mid"]) > 0
        mid_index = self.graphs_combo.findData("mid")

        if mid_index != -1:
            item = self.graphs_combo.model().item(mid_index)
            item.setEnabled(has_mid_population)

            if has_mid_population:
                self.graphs_combo.setItemText(mid_index, "mid")
            else:
                self.graphs_combo.setItemText(mid_index, "mid (early stop)")

        self.graphs_combo.setCurrentIndex(0)
        self.graph.plot_fitness(result.metrics["bestFitnesses"])
