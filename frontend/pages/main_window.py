from PySide6.QtWidgets import QMainWindow, QWidget, QStackedWidget, QVBoxLayout, QLabel, QPushButton
from frontend import AppState
from frontend.pages import MainPage
from frontend.pages import MenuPage
from frontend.pages import RunPage

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.state = AppState()
        
        # self.resize(700, 500)
        self.setWindowTitle("Metaheuristic Algorithm Simulator V2.0")
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
