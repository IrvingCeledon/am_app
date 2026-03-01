from PySide6.QtCore import Qt, Signal
from PySide6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QPushButton
from frontend.utils import centered_in_h, set_expandable_width_lim


class MainPage(QWidget):
    go_to_settings = Signal()
    go_to_run_page = Signal()
    
    def __init__(self):
        super().__init__()
        
        self._build_ui()
        self._connect_signals()
        
    def _build_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(12)
        layout.setContentsMargins(40, 40, 40, 40)
        
        self.title = QLabel("Metaheuristic Algorithm Simulator")
        self.title.setAlignment(Qt.AlignCenter)
        self.title.setStyleSheet("font-size: 28px;")
        
        self.start_btn = QPushButton("Start")
        self.settings_btn = QPushButton("Settings")
        self.exit_btn = QPushButton("Exit")  
        
        self.start_btn.setObjectName("start_btn")
        self.settings_btn.setObjectName("settings_btn")
        self.exit_btn.setObjectName("exit_btn")

        layout.addStretch()
        
        layout.addWidget(self.title)
        layout.addSpacing(30)
        
        self._set_menu_button(layout, self.start_btn)
        self._set_menu_button(layout, self.settings_btn)
        self._set_menu_button(layout, self.exit_btn)
        
        layout.addStretch() 
    
    def _set_menu_button(self, layout, btn):
        layout.addLayout(centered_in_h(btn))
        set_expandable_width_lim(btn)
    
    def _connect_signals(self):
        self.settings_btn.clicked.connect(self.go_to_settings.emit)
        self.start_btn.clicked.connect(self.go_to_run_page.emit)
        self.exit_btn.clicked.connect(QApplication.quit)
