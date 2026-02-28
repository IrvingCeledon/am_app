from PySide6.QtWidgets import QMainWindow, QWidget, QStackedWidget, QVBoxLayout, QLabel, QPushButton
from frontend.state import AppState
from .main_page import MainPage
from .menu_page import MenuPage
from .run_page import RunPage

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.state = AppState()
        
        # self.resize(700, 500)    
        self.setWindowTitle("Metaheuristic Algorithm Simulator V2") 
        self.setMinimumSize(700, 500)

        self.stack = QStackedWidget()
        self.setCentralWidget(self.stack)
        
        self._init_pages()
        
    def _init_pages(self): 
        self.main_page = MainPage(self.stack)
        self.menu_page = MenuPage(self.state, self.stack)
        self.run_page = RunPage(self.state, self.stack)
        
        self.stack.addWidget(self.main_page)
        self.stack.addWidget(self.menu_page)
        self.stack.addWidget(self.run_page)
