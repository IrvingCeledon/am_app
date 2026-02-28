from PySide6.QtWidgets import QWidget

from PySide6.QtCore import Signal

class SettingsPage(QWidget):
    def __init__(self, app_state, settings_controller):
        super().__init__()
        
        self.state = app_state
        self.settings = settings_controller
        self._i_work()
        
    def _i_work(self):
        print("I work.")
