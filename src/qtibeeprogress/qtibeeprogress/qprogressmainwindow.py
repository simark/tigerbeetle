from PyQt5 import Qt
from qtibeeprogress import utils
from qtibeeprogress.qprogresswidget import QProgressWidget


class QProgressMainWindow(Qt.QMainWindow, utils.QtUiLoad):
    def __init__(self):
        super().__init__()

        self._setup_ui()

    def _add_progress_widget(self):
        layout = self._widget_central.layout()
        widget = QProgressWidget(5, 3, 15)
        layout.insertWidget(0, widget)

    def _setup_ui(self):
        self._load_ui('main_wnd_progress')
        self._add_progress_widget()
        self.adjustSize()

    def on_update_progress(self, progress):
        # ignore if invalid
        if not progress.is_valid():
            return
