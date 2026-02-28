from PySide6.QtWidgets import QWidget
from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import mplcursors

class GraphContainer(FigureCanvas):
    def __init__(self):
        super().__init__(Figure())
        self.ax = self.figure.add_subplot(111)
        self.cursor = None

    def _setup_axes(self, xlabel="", ylabel="", title=""):
        self.ax.set_xlabel(xlabel)
        self.ax.set_ylabel(ylabel)
        self.ax.set_title(title)

    def _attach_cursor(self, artist):
        if self.cursor:
            self.cursor.remove()
            self.cursor = None

        self.cursor = mplcursors.cursor(artist, hover=mplcursors.HoverMode.Transient)

        @self.cursor.connect("add")
        def on_add(sel):
            x, y = sel.target
            sel.annotation.set_text(f"({x:.3f}, {y:.3f})")
            sel.annotation.get_bbox_patch().set(alpha=0.9)

    def plot_fitness(self, best_fitnesses):
        self.ax.clear()
        gens = range(1, len(best_fitnesses)+1)
        line, = self.ax.plot(gens, best_fitnesses)  # line
        sc = self.ax.scatter(gens, best_fitnesses, s=15)  # points
        self._attach_cursor(sc)
        self._setup_axes("Generation", "Best Fitness", "Fitness over Generations")
        self.draw()

    def plot_population(self, population, color="blue", title="Population"):
        self.ax.clear()
        xs, ys = zip(*population)
        sc = self.ax.scatter(xs, ys, c=color, alpha=0.4, s=20)
        self._attach_cursor(sc)
        self._setup_axes("X", "Y", title)
        self.draw()

    # populations: tuplas list -> (population, color, label)
    def plot_populations_all(self, populations):
        self.ax.clear()

        artists = []

        for pop, color, label in populations:
            if not pop:
                continue
            xs, ys = zip(*pop)
            sc = self.ax.scatter(xs, ys, c=color, alpha=0.4, s=20, label=label)
            artists.append(sc)

        self._setup_axes("X", "Y", "All Populations")
        if artists:
            self.ax.legend()
            self._attach_cursor(artists)

        self.draw()
