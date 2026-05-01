from PySide6.QtWidgets import (
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QComboBox,
    QCheckBox,
    QFrame
)
from PySide6.QtCore import Qt

import frontend.utils as ui


class StandardProblemPanel(QWidget):
    """
    Panel for standard optimization benchmarks (e.g., Sphere, Ackley, Rosenbrock).
    Handles dynamic ranges and optimization type (Minimize/Maximize).
    """
    def __init__(self):
        super().__init__()
        self._build_ui()

    def _build_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)

        # X range inputs
        x_layout = QHBoxLayout()
        self.x_min = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        self.x_max = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        x_layout.addWidget(QLabel("X min:"))
        x_layout.addWidget(self.x_min)
        x_layout.addWidget(QLabel("X max:"))
        x_layout.addWidget(self.x_max)

        # Y range inputs
        y_layout = QHBoxLayout()
        self.y_min = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        self.y_max = ui.create_double_spin_box(-1000.0, 1000.0, 0.01)
        y_layout.addWidget(QLabel("Y min:"))
        y_layout.addWidget(self.y_min)
        y_layout.addWidget(QLabel("Y max:"))
        y_layout.addWidget(self.y_max)

        # Optimization type (Minimize or Maximize)
        opt_layout = QHBoxLayout()
        self.opt_type_combo = QComboBox()
        self.opt_type_combo.addItem("Minimize", userData=True)
        self.opt_type_combo.addItem("Maximize", userData=False)
        opt_layout.addWidget(QLabel("Optimization:"))
        opt_layout.addWidget(self.opt_type_combo)

        layout.addLayout(x_layout)
        layout.addLayout(y_layout)
        layout.addLayout(opt_layout)

    def update_ui(self, data: dict):
        """Updates the spin boxes according to the selected benchmark limits."""
        self.x_min.setValue(data.get("x_min", -1000.0))
        self.x_max.setValue(data.get("x_max", 1000.0))
        self.y_min.setValue(data.get("y_min", -1000.0))
        self.y_max.setValue(data.get("y_max", 1000.0))
        self.opt_type_combo.setCurrentIndex(0)

    def on_dims_changed(self, dims: int):
        """Enables or disables Y inputs based on the number of dimensions."""
        is_2d = (dims == 2)
        self.y_min.setEnabled(is_2d)
        self.y_max.setEnabled(is_2d)

    def get_params(self):
        """Packs standard panel parameters for the backend."""
        return {
            "problem_type": "standard",
            "x_min": self.x_min.value(),
            "x_max": self.x_max.value(),
            "y_min": self.y_min.value(),
            "y_max": self.y_max.value(),
            "minimize": self.opt_type_combo.currentData()
        }


class IKProblemPanel(QWidget):
    """
    Specific panel for the Inverse Kinematics (IK) problem of the 5-DOF Stanford Arm.
    Gathers spatial target, quaternion orientation, and joint continuity data.
    """
    def __init__(self):
        super().__init__()
        self._build_ui()

    def _build_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)

        # Informational label
        info_label = QLabel(
            "<b>5-DOF Arm (Modified Stanford)</b><br>"
            "<span style='font-size:10px; color:#555;'>"
            "Forced search limits in Backend:<br>"
            "Rotational (q1, q2, q4, q5): [-π, π]<br>"
            "Prismatic (q3): [0.1, 1.0] m</span>"
        )
        info_label.setTextFormat(Qt.RichText)
        layout.addWidget(info_label)

        line = QFrame()
        line.setFrameShape(QFrame.HLine)
        line.setFrameShadow(QFrame.Sunken)
        layout.addWidget(line)

        # Spatial Target (X, Y, Z)
        target_layout = QHBoxLayout()
        self.ik_tx = ui.create_double_spin_box(-2.0, 2.0, 0.1, 0.5)
        self.ik_ty = ui.create_double_spin_box(-2.0, 2.0, 0.1, 0.5)
        self.ik_tz = ui.create_double_spin_box(-2.0, 2.0, 0.1, 0.5)
        target_layout.addWidget(QLabel("Target (X,Y,Z):"))
        target_layout.addWidget(self.ik_tx)
        target_layout.addWidget(self.ik_ty)
        target_layout.addWidget(self.ik_tz)
        layout.addLayout(target_layout)

        # Extra Feature: Quaternions (Orientation Penalty)
        self.enable_quat_cb = QCheckBox("Full Pose (30 pts)")
        self.enable_quat_cb.toggled.connect(self._toggle_quat_inputs)
        layout.addWidget(self.enable_quat_cb)

        self.quat_layout = QHBoxLayout()
        self.ik_qw = ui.create_double_spin_box(-1.0, 1.0, 0.1, 1.0, enable=False)
        self.ik_qx = ui.create_double_spin_box(-1.0, 1.0, 0.1, 0.0, enable=False)
        self.ik_qy = ui.create_double_spin_box(-1.0, 1.0, 0.1, 0.0, enable=False)
        self.ik_qz = ui.create_double_spin_box(-1.0, 1.0, 0.1, 0.0, enable=False)

        self.quat_layout.addWidget(QLabel("Quat. (w,x,y,z):"))
        self.quat_layout.addWidget(self.ik_qw)
        self.quat_layout.addWidget(self.ik_qx)
        self.quat_layout.addWidget(self.ik_qy)
        self.quat_layout.addWidget(self.ik_qz)
        layout.addLayout(self.quat_layout)

        # Extra Feature: Joint Continuity (Motion Penalty)
        self.enable_prev_cb = QCheckBox("Joint Continuity (20 pts)")
        self.enable_prev_cb.toggled.connect(self._toggle_prev_inputs)
        layout.addWidget(self.enable_prev_cb)

        self.prev_layout = QHBoxLayout()
        self.ik_pq1 = ui.create_double_spin_box(-3.15, 3.15, 0.1, 0.0, enable=False)
        self.ik_pq2 = ui.create_double_spin_box(-3.15, 3.15, 0.1, 0.0, enable=False)
        self.ik_pq3 = ui.create_double_spin_box(0.1, 1.0, 0.1, 0.5, enable=False)
        self.ik_pq4 = ui.create_double_spin_box(-3.15, 3.15, 0.1, 0.0, enable=False)
        self.ik_pq5 = ui.create_double_spin_box(-3.15, 3.15, 0.1, 0.0, enable=False)

        self.prev_layout.addWidget(QLabel("Prev Posture:"))
        self.prev_layout.addWidget(self.ik_pq1)
        self.prev_layout.addWidget(self.ik_pq2)
        self.prev_layout.addWidget(self.ik_pq3)
        self.prev_layout.addWidget(self.ik_pq4)
        self.prev_layout.addWidget(self.ik_pq5)
        layout.addLayout(self.prev_layout)

        layout.addStretch()

    def _toggle_quat_inputs(self, checked: bool):
        """Enables or disables quaternion inputs based on the checkbox."""
        self.ik_qw.setEnabled(checked)
        self.ik_qx.setEnabled(checked)
        self.ik_qy.setEnabled(checked)
        self.ik_qz.setEnabled(checked)

    def _toggle_prev_inputs(self, checked: bool):
        """Enables or disables previous posture inputs based on the checkbox."""
        self.ik_pq1.setEnabled(checked)
        self.ik_pq2.setEnabled(checked)
        self.ik_pq3.setEnabled(checked)
        self.ik_pq4.setEnabled(checked)
        self.ik_pq5.setEnabled(checked)

    def update_ui(self, data: dict):
        """Fills the inputs using the specific IK defaults from benchmarks.py."""
        defaults = data.get("ik_defaults", {})
        if defaults:
            t_xyz = defaults.get("target_xyz", [0.5, 0.5, 0.5])
            self.ik_tx.setValue(t_xyz[0])
            self.ik_ty.setValue(t_xyz[1])
            self.ik_tz.setValue(t_xyz[2])

            t_q = defaults.get("target_quat", [1.0, 0.0, 0.0, 0.0])
            self.ik_qw.setValue(t_q[0])
            self.ik_qx.setValue(t_q[1])
            self.ik_qy.setValue(t_q[2])
            self.ik_qz.setValue(t_q[3])

            p_p = defaults.get("prev_posture", [0.0, 0.0, 0.5, 0.0, 0.0])
            self.ik_pq1.setValue(p_p[0])
            self.ik_pq2.setValue(p_p[1])
            self.ik_pq3.setValue(p_p[2])
            self.ik_pq4.setValue(p_p[3])
            self.ik_pq5.setValue(p_p[4])

    def get_params(self):
        """Packs IK specific parameters to be injected directly into the IKEvaluator."""
        return {
            "problem_type": "ik",
            "ik_target_xyz": [self.ik_tx.value(), self.ik_ty.value(), self.ik_tz.value()],
            "ik_use_quat": self.enable_quat_cb.isChecked(),
            "ik_target_quat": [self.ik_qw.value(), self.ik_qx.value(), self.ik_qy.value(), self.ik_qz.value()],
            "ik_use_prev": self.enable_prev_cb.isChecked(),
            "ik_prev_posture": [self.ik_pq1.value(), self.ik_pq2.value(), self.ik_pq3.value(), self.ik_pq4.value(), self.ik_pq5.value()]
        }
