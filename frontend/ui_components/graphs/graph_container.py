from PySide6.QtWidgets import QWidget
from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import mplcursors
import numpy as np

from frontend.config import BENCHMARKS

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
            sel.annotation.get_bbox_patch().set(alpha=0.5)

    def plot_fitness(self, best_fitnesses):
        self.ax.clear()
        gens = range(1, len(best_fitnesses)+1)
        line, = self.ax.plot(gens, best_fitnesses)  
        sc = self.ax.scatter(gens, best_fitnesses, s=15)  
        self._attach_cursor(sc)
        self._setup_axes("Generation", "Best Fitness", "Fitness over Generations")
        self.draw()

    def _draw_background(self, dim, params):
        """Dibuja la función matemática real de fondo"""
        if not params: return
        
        func_id = params.get("fitness")
        if not func_id or func_id not in BENCHMARKS: return
        
        func = BENCHMARKS[func_id]["func"]
        x_min, x_max = params["x_min"], params["x_max"]
        
        # Calculamos un margen visual del 5% del tamaño del dominio
        pad_x = (x_max - x_min) * 0.05
        
        if dim == 1:
            # Ampliamos la evaluación matemática para que dibuje la línea en el padding
            X = np.linspace(x_min - pad_x, x_max + pad_x, 200)
            Y = [func([x]) for x in X]
            self.ax.plot(X, Y, color='gray', linestyle='--', zorder=1)
            
            self.ax.set_xlim(x_min - pad_x, x_max + pad_x)
            
        elif dim == 2:
            y_min, y_max = params["y_min"], params["y_max"]
            pad_y = (y_max - y_min) * 0.05
            
            # Ampliamos la malla generadora para que el mapa de calor cubra el padding
            X = np.linspace(x_min - pad_x, x_max + pad_x, 100)
            Y = np.linspace(y_min - pad_y, y_max + pad_y, 100)
            XX, YY = np.meshgrid(X, Y)
            
            ZZ = np.zeros_like(XX)
            # Evaluamos la función C++ para cada punto de la malla extendida
            for i in range(XX.shape[0]):
                for j in range(XX.shape[1]):
                    ZZ[i, j] = func([XX[i, j], YY[i, j]])
                    
            self.ax.contourf(XX, YY, ZZ, levels=40, cmap='viridis', alpha=0.6, zorder=1)
            
            self.ax.set_xlim(x_min - pad_x, x_max + pad_x)
            self.ax.set_ylim(y_min - pad_y, y_max + pad_y)

            # Opcional: Puedes dibujar un rectángulo sutil para marcar dónde 
            # está el verdadero límite matemático de la función.
            import matplotlib.patches as patches
            rect = patches.Rectangle((x_min, y_min), x_max - x_min, y_max - y_min, 
                                     linewidth=1.5, edgecolor='black', facecolor='none', linestyle=':', zorder=2)
            self.ax.add_patch(rect)

    def plot_population(self, population, color="blue", title="Population", params=None):
        self.ax.clear()
        if not population:
            self.draw()
            return

        dim = len(population[0])
        self._draw_background(dim, params)

        if dim == 1:
            # Usando tu idea: Evaluamos el Fitness para ponerlo en el eje Y
            func = BENCHMARKS[params["fitness"]]["func"] if params else lambda x: 0
            xs = [ind[0] for ind in population]
            ys = [func([x]) for x in xs]
            sc = self.ax.scatter(xs, ys, c=color, edgecolors='black', alpha=0.5, s=60, zorder=2)
            self._attach_cursor(sc)
            self._setup_axes("X", "Fitness f(X)", f"{title} (1D)")

        elif dim == 2:
            xs, ys = zip(*population)
            sc = self.ax.scatter(xs, ys, c=color, edgecolors='black', alpha=0.5, s=50, zorder=2)
            self._attach_cursor(sc)
            self._setup_axes("X", "Y", title)

        else:
            self.ax.get_yaxis().set_visible(True)
            self.ax.text(0.5, 0.5, f"Cannot show dispersion of functions with >2D (Current: {dim}D)", 
                         horizontalalignment='center', verticalalignment='center', 
                         transform=self.ax.transAxes, fontsize=12, color='red')
            self._setup_axes("", "", title)

        self.draw()

    def plot_populations_all(self, populations, params=None):
        self.ax.clear()

        dim = 0
        for pop, _, _ in populations:
            if pop:
                dim = len(pop[0])
                break
                
        if dim == 0:
            self.draw()
            return

        if dim > 2:
            self.ax.get_yaxis().set_visible(True)
            self.ax.text(0.5, 0.5, f"Cannot show dispersion of functions with > 2D (Current: {dim}D)", 
                         horizontalalignment='center', verticalalignment='center', 
                         transform=self.ax.transAxes, fontsize=12, color='red')
            self._setup_axes("", "", "All Populations")
            self.draw()
            return

        self._draw_background(dim, params)
        artists = []
        func = BENCHMARKS[params["fitness"]]["func"] if params else lambda x: 0

        for pop, color, label in populations:
            if not pop: continue
                
            if dim == 1:
                xs = [ind[0] for ind in pop]
                ys = [func([x]) for x in xs]
            else:
                xs, ys = zip(*pop)
                
            sc = self.ax.scatter(xs, ys, c=color, edgecolors='black', alpha=0.50, s=40, label=label, zorder=2)
            artists.append(sc)

        self._setup_axes("X", "Fitness f(X)" if dim == 1 else "Y", f"All Populations ({dim}D)")
        if artists:
            self.ax.legend()
            self._attach_cursor(artists)

        self.draw()
