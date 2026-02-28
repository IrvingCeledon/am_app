class SettingsController:
    def __init__(self, app_state):
        self.state = app_state
        self._i_work()
        
    def _i_work(self):
        print("I work.")
