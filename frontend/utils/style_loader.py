from pathlib import Path

def load_stylesheet(app, *paths):
    style = ""
    for p in paths:
        with open(p) as f:
            style += f.read()
    app.setStyleSheet(style)

