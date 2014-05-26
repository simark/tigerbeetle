from PyQt5 import Qt
from PyQt5 import QtGui
from qtibeeprogress import utils
from qtibeeprogress.qprogresswidget import QProgressWidget


class QProgressMainWindow(Qt.QMainWindow, utils.QtUiLoad):
    def __init__(self):
        super().__init__()

        self._setup_ui()

    def _add_progress_widget(self):
        layout = self._widget_central.layout()
        self._progress_widget = QProgressWidget(5, 3, 15)
        layout.insertWidget(0, self._progress_widget)

    def _setup_labels(self):
        self._labels = [
            self._lbl_begin_time,
            self._lbl_end_time,
            self._lbl_cur_time,
            self._lbl_processed_events,
            self._lbl_state_changes,
        ]

    def _setup_edits(self):
        self._edits = [
            self._txt_traces,
            self._txt_state_providers,
        ]

    def _setup_style(self):
        palette = QtGui.QPalette()
        palette.setColor(QtGui.QPalette.WindowText, QtGui.QColor('#888'))

        caption_labels = [
            self._lbl_begin_time_caption,
            self._lbl_end_time_caption,
            self._lbl_cur_time_caption,
            self._lbl_processed_events_caption,
            self._lbl_traces_caption,
            self._lbl_state_changes_caption,
            self._lbl_state_providers_caption,
        ]

        for label in caption_labels:
            label.setPalette(palette)

    def _setup_ui(self):
        self._load_ui('main_wnd_progress')
        self._setup_style()
        self._setup_labels()
        self._setup_edits()
        self._add_progress_widget()
        self.adjustSize()

    def on_update_progress(self, progress):
        # ignore if invalid
        if not progress.is_valid():
            return

    def set_waiting(self):
        waiting_text = 'Waiting for data...'

        for lbl in self._labels:
            lbl.setText(waiting_text)

        for edit in self._edits:
            edit.setPlainText(waiting_text)

    def set_progress_update(self, progress_update):
        pass
