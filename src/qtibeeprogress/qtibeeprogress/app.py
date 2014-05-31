import os
import sys
import logging
import platform
import argparse
from PyQt5 import Qt
from PyQt5 import QtGui
from PyQt5 import QtCore
from qtibeeprogress import __version__
from qtibeeprogress.qprogressmainwindow import QProgressMainWindow
from qtibeeprogress.qupdatelistener import QUpdateListener


logger = logging.getLogger(__name__)


class _App(Qt.QApplication):
    start_update_listener = QtCore.pyqtSignal()

    def __init__(self, args, addr, quit_after):
        super().__init__(args[0:1])

        self._addr = addr
        self._quit_after = quit_after

        self._start()

    def stop(self):
        logger.info('Stopping application')

        self._update_listener.stop()
        self._update_listener_thread.quit()

        logger.debug('Joining update listener thread')

        self._update_listener_thread.wait()
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
        palette.setColor(QtGui.QPalette.BrightText, QtCore.Qt.red)
        palette.setColor(QtGui.QPalette.Highlight, QtGui.QColor('#8b1727'))
        palette.setColor(QtGui.QPalette.HighlightedText, QtGui.QColor('#e5a9b1'))

        self.setPalette(palette)

    def _setup_font(self):
        pass

    def _setup_style(self):
        self._setup_palette()
        self._setup_font()

    def _on_update_available(self, update):
        if update.is_done() and self._quit_after:
            logger.debug('Build completed: quitting')
            Qt.QTimer.singleShot(0, self.stop)

            return

        self._main_wnd_progress.set_progress_update(update)

    def _on_zmq_error(self):
        msg = 'error: cannot connect to "{}"'.format(self._addr)
        print(msg, file=sys.stderr)
        Qt.QTimer.singleShot(0, self.stop)

    def _start_update_listener(self):
        # create update listener thread
        self._update_listener_thread = Qt.QThread()

        # start update listener thread
        logger.debug('Starting update listener thread')
        self._update_listener_thread.start()

        # create update listener
        self._update_listener = QUpdateListener(self._addr, self._quit_after)

        # move update listener to update listener thread
        logger.debug('Moving update listener to its own thread')
        self._update_listener.moveToThread(self._update_listener_thread)

        # connect stuff
        ul = self._update_listener
        self.start_update_listener.connect(ul.start)
        ul.update_available.connect(self._on_update_available)
        ul.zmq_error.connect(self._on_zmq_error)

        # start update listener
        self.start_update_listener.emit()

    def _on_about_to_quit(self):
        self.stop()

    def _start(self):
        logger.info('Starting application')

        self.aboutToQuit.connect(self._on_about_to_quit)
        self._setup_python_timer()
        self._setup_style()
        self._show_progress_main_window()
        self._start_update_listener()


def _register_sigint(app):
    if platform.system() == 'Linux':
        def handler(signal, frame):
            logger.debug('Got SIGINT')
            app.stop()

        import signal
        signal.signal(signal.SIGINT, handler)


def _configure_logging(args):
    logging.basicConfig(level=logging.DEBUG)


def _parse_args():
    parser = argparse.ArgumentParser()

    version = 'qtibeeprogress v{}'.format(__version__)
    parser.add_argument('-V, --version', action='version',
                        help='print version and quit',
                        version=version)
    parser.add_argument('-q', action='store_true',
                        help='quit when finished')
    parser.add_argument('addr', type=str, nargs=1,
                        help='address to connect to')

    args = parser.parse_args()

    return args


def run():
    args = _parse_args()

    _configure_logging(args)

    logger.debug('Creating application')
    app = _App(sys.argv, args.addr[0], args.q)

    _register_sigint(app)

    logger.debug('Executing application')
    return app.exec_()
