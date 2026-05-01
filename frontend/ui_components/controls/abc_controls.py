from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QLabel, QCheckBox
from frontend.utils import create_spin_box
from .controls import Controls

class ABCControls(Controls):        
    def __init__(self):
        super().__init__()
        
    def build_layout(self):
        upper_controls = QHBoxLayout()
        upper_controls.setSpacing(5)

        upper_controls.addLayout(self._build_size_section())
        upper_controls.addLayout(self._build_domains_section())
        upper_controls.addLayout(self._build_abc_miscellaneous_section())

        return upper_controls

    def _build_abc_miscellaneous_section(self):           
        abc_miscellaneous_section = self._build_miscellaneous_section()
        
        # Flag to use gbest
        self.use_gbest = QCheckBox("Use GBest:")
        
        # Limit (patience of foood sources)
        limit_layout = QHBoxLayout()
        self.limit_spin = create_spin_box(10, 1000, 10, 100)
        
        limit_layout.addWidget(QLabel("Limit (Abandonment):"))
        limit_layout.addWidget(self.limit_spin)

        abc_miscellaneous_section.addWidget(self.use_gbest)
        abc_miscellaneous_section.addLayout(limit_layout)

        return abc_miscellaneous_section
            
    def get_params(self):
        # Get base parameters (size, domains, miscellaneous)
        params = super().get_params()

        # Add ABC specific parameters
        params.update({
            "use_gbest": self.use_gbest.isChecked(),
            "limit": self.limit_spin.value()
        })

        return params
