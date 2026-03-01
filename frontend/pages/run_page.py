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
from frontend.ui_components.controls.ga_controls import GAControls
from frontend.ui_components.controls.cga_controls import CGAControls


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

        self._build_ui()
        self._connect_signals()

    def _build_ui(self):
        main_layout = QVBoxLayout(self)
        upper_controls = QHBoxLayout()

        # 1. Creamos el Stack (El mazo de cartas)
        self.controls_stack = QStackedWidget()
        self.controls_stack.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Minimum)

        # 2. Creamos y guardamos las instancias de los controles una sola vez
        self.ga_controls = GAControls()
        self.cga_controls = CGAControls()

        # 2. GA Widget - Alineado arriba
        self.ga_widget = QWidget()
        ga_layout = self.ga_controls.build_layout()
        ga_layout.setAlignment(Qt.AlignmentFlag.AlignTop)
        self.ga_widget.setLayout(ga_layout)

        # 3. CGA Widget - Alineado arriba
        self.cga_widget = QWidget()
        cga_layout = self.cga_controls.build_layout()
        cga_layout.setAlignment(Qt.AlignmentFlag.AlignTop)
        self.cga_widget.setLayout(cga_layout)

        # 4. Añadimos las "cartas" al mazo
        self.controls_stack.addWidget(self.ga_widget)
        self.controls_stack.addWidget(self.cga_widget)

        # 5. Definimos el control activo actual por defecto
        self.controls = self.ga_controls

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
        self.algorithm_combo.addItems(["GA", "CGA"])

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

    def _update_graph_by_selection(self, index):
        if not self._last_result or index == -1:
            self.graphs_combo.setCurrentIndex(0)
            return

        internal_data = self.graphs_combo.itemData(index)
        snaps = self._last_result.snapshots

        if internal_data == "none":
            self.graph.plot_fitness(self._last_result.metrics["bestFitnesses"])

        elif internal_data == "all":
            plots = [(snaps["initial"], "blue", "Initial")]
            if len(snaps["mid"]) > 0:
                plots.append((snaps["mid"], "orange", "Mid"))
            plots.append((snaps["final"], "green", "Final"))

            self.graph.plot_populations_all(plots)

        elif internal_data == "initial":
            self.graph.plot_population(snaps["initial"], "blue", "Initial")

        elif internal_data == "mid":
            self.graph.plot_population(snaps["mid"], "orange", "Mid")

        elif internal_data == "final":
            self.graph.plot_population(snaps["final"], "green", "Final")

    def _on_algorithm_changed(self, algorithm: str):
        if algorithm == "GA":
            self.controls_stack.setCurrentWidget(self.ga_widget)
            self.controls = self.ga_controls
        elif algorithm == "CGA":
            self.controls_stack.setCurrentWidget(self.cga_widget)
            self.controls = self.cga_controls
