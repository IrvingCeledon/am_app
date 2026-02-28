from PySide6.QtWidgets import QMainWindow, QStackedWidget
from .main_page import MainPage
from .run_page import RunPage
from .settings_page import SettingsPage

class MainWindow(QMainWindow):
    MAIN = 0
    SETTINGS = 1
    RUN = 2

    def __init__(self, app_state, settings_controller, run_controller):
        super().__init__()
        
        self.state = app_state
        self.settings = settings_controller
        self.run_controller = run_controller
        
        # self.resize(700, 500)
        self.setWindowTitle("Metaheuristic Algorithm Simulator V2.1")
        self.setMinimumSize(700, 500)
        
        self.stack = QStackedWidget()
        self.setCentralWidget(self.stack)
        
        self._init_pages()
        self._connect_signals()
        
    def _init_pages(self): 
        self.main_page = MainPage()
        self.settings_page = SettingsPage(self.state, self.settings)
        self.run_page = RunPage()
        
        self.stack.addWidget(self.main_page)
        self.stack.addWidget(self.settings_page)
        self.stack.addWidget(self.run_page)
        
    def _connect_signals(self):
        self.main_page.go_to_settings.connect( self._go_to_settings )
        self.main_page.go_to_run_page.connect( self._go_to_run_page )
        self.run_page.backRequested.connect( self._back_requested )
        # self.run_page.go_back.connect(lambda: self.stack.setCurrentIndex(MAIN_PAGE))
        
        self.run_page.runRequested.connect(self.run_controller.run)
        self.run_controller.resultReady.connect(self.run_page.show_result)
        
    def _go_to_settings(self):
        self.stack.setCurrentIndex(self.SETTINGS)
        
    def _go_to_run_page(self):
        self.stack.setCurrentIndex(self.RUN)
        
    def _back_requested(self):
        self.stack.setCurrentIndex(self.MAIN)
