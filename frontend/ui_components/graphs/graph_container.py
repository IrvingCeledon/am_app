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
            # 1. Si el elemento es una Línea en 3D (Line3D)
            if hasattr(sel.artist, 'get_data_3d'):
                xs, ys, zs = sel.artist.get_data_3d()
                # Las líneas interpolan, por lo que el índice puede ser flotante. Lo redondeamos.
                idx = int(round(sel.index))
                sel.annotation.set_text(f"({xs[idx]:.3f}, {ys[idx]:.3f}, {zs[idx]:.3f})")

            # 2. Si el elemento es un Punto de Dispersión 3D (Path3DCollection)
            elif hasattr(sel.artist, '_offsets3d'):
                xs, ys, zs = sel.artist._offsets3d
                idx = int(round(sel.index))
                sel.annotation.set_text(f"({xs[idx]:.3f}, {ys[idx]:.3f}, {zs[idx]:.3f})")

            # 3. Fallback para tus gráficos estándar en 1D y 2D
            elif hasattr(sel.target, '__getitem__') and len(sel.target) >= 2:
                x, y = sel.target[:2]
                sel.annotation.set_text(f"({x:.3f}, {y:.3f})")

            # Estilo del recuadro
            sel.annotation.get_bbox_patch().set(alpha=0.5)

    def plot_fitness(self, best_fitnesses):
        self.figure.clear()
        self.ax = self.figure.add_subplot(111)

        gens = range(1, len(best_fitnesses)+1)
        line, = self.ax.plot(gens, best_fitnesses, color='#2196F3')
        sc = self.ax.scatter(gens, best_fitnesses, s=15, color='#0D47A1')
        self._attach_cursor(sc)
        self._setup_axes("Generation", "Best Fitness", "Fitness over Generations")
        self.draw()

    def _draw_background(self, dim, params):
        """Dibuja la función matemática real de fondo"""
        if not params: return

        func_id = params.get("fitness")
        if not func_id or func_id not in BENCHMARKS: return

        # Corrección: Evitamos KeyError usando .get()
        func = BENCHMARKS[func_id].get("func")
        if not func: return

        x_min, x_max = params["x_min"], params["x_max"]

        # Calculamos un margen visual del 5% del tamaño del dominio
        pad_x = (x_max - x_min) * 0.05

        if dim == 1:
            X = np.linspace(x_min - pad_x, x_max + pad_x, 200)
            Y = [func([x]) for x in X]
            self.ax.plot(X, Y, color='gray', linestyle='--', zorder=1)

            self.ax.set_xlim(x_min - pad_x, x_max + pad_x)

        elif dim == 2:
            y_min, y_max = params["y_min"], params["y_max"]
            pad_y = (y_max - y_min) * 0.05

            X = np.linspace(x_min - pad_x, x_max + pad_x, 100)
            Y = np.linspace(y_min - pad_y, y_max + pad_y, 100)
            XX, YY = np.meshgrid(X, Y)

            ZZ = np.zeros_like(XX)
            for i in range(XX.shape[0]):
                for j in range(XX.shape[1]):
                    ZZ[i, j] = func([XX[i, j], YY[i, j]])

            self.ax.contourf(XX, YY, ZZ, levels=40, cmap='viridis', alpha=0.6, zorder=1)

            self.ax.set_xlim(x_min - pad_x, x_max + pad_x)
            self.ax.set_ylim(y_min - pad_y, y_max + pad_y)

            import matplotlib.patches as patches
            rect = patches.Rectangle((x_min, y_min), x_max - x_min, y_max - y_min,
                                     linewidth=1.5, edgecolor='black', facecolor='none', linestyle=':', zorder=2)
            self.ax.add_patch(rect)

    def plot_population(self, population, color="blue", title="Population", params=None):
        self.figure.clear()
        self.ax = self.figure.add_subplot(111)

        if not population:
            self.draw()
            return

        dim = len(population[0])

        # Corrección: Bloquear N-Dimensionales antes de intentar dibujar el fondo
        if dim > 2:
            self.ax.get_yaxis().set_visible(True)
            self.ax.text(0.5, 0.5, f"Cannot show dispersion of functions with >2D\n(Current: {dim}D)",
                         horizontalalignment='center', verticalalignment='center',
                         transform=self.ax.transAxes, fontsize=12, color='red')
            self._setup_axes("", "", title)
            self.draw()
            return

        self._draw_background(dim, params)

        if dim == 1:
            func = BENCHMARKS[params["fitness"]].get("func", lambda x: 0) if params else lambda x: 0
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

        self.draw()

    def plot_populations_all(self, populations, params=None):
        self.figure.clear()
        self.ax = self.figure.add_subplot(111)

        dim = 0
        for pop, _, _ in populations:
            if pop:
                dim = len(pop[0])
                break

        if dim == 0:
            self.draw()
            return

        # Corrección: Bloqueo de N-Dimensiones
        if dim > 2:
            self.ax.get_yaxis().set_visible(True)
            self.ax.text(0.5, 0.5, f"Cannot show dispersion of functions with > 2D\n(Current: {dim}D)",
                         horizontalalignment='center', verticalalignment='center',
                         transform=self.ax.transAxes, fontsize=12, color='red')
            self._setup_axes("", "", "All Populations")
            self.draw()
            return

        self._draw_background(dim, params)
        artists = []
        func = BENCHMARKS[params["fitness"]].get("func", lambda x: 0) if params else lambda x: 0

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

    def plot_robot_3d(self, best_individual, target_xyz):
        """
        Recrea la cinemática directa (DH) en Python para extraer los puntos articulares
        y grafica el brazo robótico en 3D junto con el punto objetivo.
        """
        self.figure.clear()
        self.ax = self.figure.add_subplot(111, projection='3d')

        q = best_individual

        # Tabla D-H (theta, alpha, b, d)
        dh_params = [
            (q[0], -np.pi/2, 0.5, 0.0),
            (q[1], np.pi/2,  0.4, 0.0),
            (0.0,  0.0,      0.0, q[2]),
            (q[3], -np.pi/2, 0.0, 0.0),
            (q[4], np.pi/2,  0.3, 0.0)
        ]

        points = [[0.0, 0.0, 0.0]]
        T = np.eye(4)

        for theta, alpha, b, d in dh_params:
            ct, st = np.cos(theta), np.sin(theta)
            ca, sa = np.cos(alpha), np.sin(alpha)

            M = np.array([
                [ct, -st*ca,  st*sa, b*ct],
                [st,  ct*ca, -ct*sa, b*st],
                [0,   sa,     ca,    d],
                [0,   0,      0,     1]
            ])
            T = T @ M
            points.append([T[0, 3], T[1, 3], T[2, 3]])

        points = np.array(points)
        xs, ys, zs = points[:, 0], points[:, 1], points[:, 2]

        # Dibujar los eslabones del brazo (plot devuelve una lista, tomamos el elemento 0)
        arm_line = self.ax.plot(xs, ys, zs, 'o-', color='#2196F3', linewidth=4, markersize=8, label='Stanford Arm')[0]

        # Resaltar la base (Origen)
        base_pt = self.ax.scatter(0, 0, 0, color='black', s=100, marker='s', label='Base')

        # Dibujar el punto objetivo deseado
        tx, ty, tz = target_xyz
        target_pt = self.ax.scatter(tx, ty, tz, color='#f44336', s=150, marker='*', label='Target (Xd, Yd, Zd)')

        # Configuración visual
        self.ax.set_xlabel('X Axis')
        self.ax.set_ylabel('Y Axis')
        self.ax.set_zlabel('Z Axis')
        self.ax.set_title('3D Kinematic Pose')
        self.ax.legend()

        self._attach_cursor([arm_line, base_pt, target_pt])

        # Forzar proporciones iguales
        max_range = np.array([xs.max()-xs.min(), ys.max()-ys.min(), zs.max()-zs.min()]).max() / 2.0
        mid_x = (xs.max()+xs.min()) * 0.5
        mid_y = (ys.max()+ys.min()) * 0.5
        mid_z = (zs.max()+zs.min()) * 0.5
        self.ax.set_xlim(mid_x - max_range, mid_x + max_range)
        self.ax.set_ylim(mid_y - max_range, mid_y + max_range)
        self.ax.set_zlim(mid_z - max_range, mid_z + max_range)

        self.draw()
