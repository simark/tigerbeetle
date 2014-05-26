import os
import sys
import logging
import platform
from PyQt5 import Qt
from PyQt5 import QtGui
from PyQt5 import QtCore
from qtibeeprogress.qprogressmainwindow import QProgressMainWindow
from qtibeeprogress.qupdatelistener import QUpdateListener


class _App(Qt.QApplication):
    def __init__(self, args):
        super().__init__(args)

        self._start()

    def stop(self):
        logging.debug('Stopping application')

        self._main_wnd_progress.close()

    def _show_progress_main_window(self):
        res = Qt.QDesktopWidget().screenGeometry()

        self._main_wnd_progress = QProgressMainWindow()
        x = (res.width() - self._main_wnd_progress.width()) // 2
        y = (res.height() - self._main_wnd_progress.height()) // 2

        self._main_wnd_progress.move(Qt.QPoint(x, y))
        self._main_wnd_progress.set_waiting()
        self._main_wnd_progress.show()

    def _setup_python_timer(self):
        # let the interpreter run sometimes to handle signals
        self._python_timer = Qt.QTimer()
        self._python_timer.start(250)
        self._python_timer.timeout.connect(lambda: None)

    def _setup_palette(self):
        self.setStyle('Fusion')

        palette = QtGui.QPalette()
        palette.setColor(QtGui.QPalette.Window, QtGui.QColor('#222'))
        palette.setColor(QtGui.QPalette.WindowText, QtGui.QColor('#ccc'))
        palette.setColor(QtGui.QPalette.Base, QtGui.QColor('#111'))
        palette.setColor(QtGui.QPalette.AlternateBase, QtGui.QColor('blue'))
        palette.setColor(QtGui.QPalette.ToolTipBase, QtCore.Qt.white)
        palette.setColor(QtGui.QPalette.ToolTipText, QtCore.Qt.white)
        palette.setColor(QtGui.QPalette.Text, QtGui.QColor('#ccc'))
        palette.setColor(QtGui.QPalette.Button, QtGui.QColor('red'))
        palette.setColor(QtGui.QPalette.ButtonText, QtCore.Qt.white)
        palette.setColor(QtGui.QPalette.BrightText, QtCore.Qt.red)
        palette.setColor(QtGui.QPalette.Highlight, QtGui.QColor('#8b1727'))
        palette.setColor(QtGui.QPalette.HighlightedText, QtGui.QColor('#e5a9b1'))

        self.setPalette(palette)

    def _setup_font(self):
        pass

    def _setup_style(self):
        self._setup_palette()
        self._setup_font()

    def _start(self):
        logging.debug('Starting application')

        self._setup_python_timer()
        self._setup_style()
        self._show_progress_main_window()


def _register_sigint(app):
    if platform.system() == 'Linux':
        def handler(signal, frame):
            app.stop()

        import signal
        signal.signal(signal.SIGINT, handler)


def _configure_logging():
    logging.basicConfig(level=logging.DEBUG)


def run():
    _configure_logging()
    app = _App(sys.argv)
    _register_sigint(app)

    return app.exec_()
