import math
from PyQt5 import Qt
from PyQt5 import QtGui
from PyQt5 import QtCore
from qtibeeprogress import config


class QProgressWidget(Qt.QWidget):
    def __init__(self, dot_size, dot_spacing, padding):
        super().__init__()

        self._setup_ui()
        self._value = 0
        self._dot_size = dot_size
        self._dot_spacing = dot_spacing
        self._padding = padding

    def _setup_ui(self):
        self.setSizePolicy(Qt.QSizePolicy(Qt.QSizePolicy.Expanding,
                                          Qt.QSizePolicy.Expanding))
        self.setMinimumSize(0, 70)

    def _draw(self, painter):
        w = self.width()
        h = self.height()
        p = self._padding
        ds = self._dot_size
        dp = self._dot_spacing

        # background
        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(QtGui.QColor('#080808'))
        painter.drawRect(0, 0, w, h)

        # some parameters
        iw = (w - (2 * p))
        ih = (h - (2 * p))

        # total dots that fit
        total_dots_w = math.floor(iw / (ds + dp))
        if ((ds + dp) * total_dots_w + ds) <= iw:
            total_dots_w += 1
        total_dots_h = math.floor(ih / (ds + dp))
        if ((ds + dp) * total_dots_h + ds) <= ih:
            total_dots_h += 1
        total_dots = total_dots_w * total_dots_h

        # total dots to draw
        draw_dots = round(total_dots * self._value)

        # draw dots
        cx = p
        cy = p
        dots_y = 0
        dots = 0
        painter.setBrush(config.theme_foreground)
        while dots < draw_dots:
            if dots_y >= total_dots_h:
                dots_y = 0
                cx += (ds + dp)
                cy = p
            painter.drawRect(cx, cy, ds, ds)
            cy += (ds + dp)
            dots_y += 1
            dots += 1

    def set_value(self, value):
        if value > 1:
            value = 1
        self._value = value
        self.update()

    def paintEvent(self, ev):
        painter = QtGui.QPainter()

        painter.begin(self)
        self._draw(painter)
        painter.end()

    def resizeEvent(self, ev):
        size = ev.size()
