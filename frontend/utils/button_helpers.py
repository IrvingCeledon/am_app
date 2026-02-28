from PySide6.QtWidgets import QSizePolicy

def set_expandable_width_lim(btn, max_width=600):
    btn.setMaximumWidth(max_width)
    btn.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
