from PySide6.QtWidgets import QWidget
from PySide6.QtCore import Signal


class SettingsPage(QWidget):
    def __init__(self, app_state, settings_controller):
        super().__init__()
