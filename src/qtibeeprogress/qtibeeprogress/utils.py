import os.path
from pkg_resources import resource_filename
from PyQt5 import Qt
from PyQt5 import uic

class QCommonDialog(Qt.QDialog):
    def __init__(self):
        super().__init__()

    def show_move(self, pos):
        self.move(pos)
        self.exec()


class QtUiLoad:
    def _load_ui(self, ui_name):
        ui_rel_path = os.path.join('dat', 'ui', '{}.ui'.format(ui_name))
        ui_path = resource_filename(__name__, ui_rel_path)
        uic.loadUi(ui_path, baseinstance=self)
