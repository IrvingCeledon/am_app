from PySide6.QtWidgets import QWidget, QVBoxLayout, QLabel, QPushButton

class MenuPage(QWidget):
    def __init__(self, state, stack):
        super().__init__()
        
        self.state = state
        self.stack = stack
        
        self._build_ui()
        self._conect_signals()
        
    def _build_ui(self):
        print("Under maintenance")
    
    def _conect_signals(self):
        print("Under maintenance")
    
