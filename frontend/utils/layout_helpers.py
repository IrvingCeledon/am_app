from PySide6.QtWidgets import QHBoxLayout

def centered_in_h(widget):
    layout = QHBoxLayout()
    
    layout.addStretch()
    layout.addWidget(widget)
    layout.addStretch()
    
    return layout
