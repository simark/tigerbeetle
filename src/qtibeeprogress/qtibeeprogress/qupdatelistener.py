import zmq
import json
import logging
from PyQt5 import Qt
from PyQt5 import QtCore
from qtibeeprogress import utils


logger = logging.getLogger(__name__)


class ProgressUpdate:
    def __init__(self, json_bytes):
        self._valid = True

        try:
            json_str = json_bytes.decode('utf-8')
            msg = json.loads(json_str)
        except:
            self._valid = False
            return

        if 'method' not in msg:
            self._valid = False
            return

        if msg['method'] != 'progress-update':
            self._valid = False
            return

        self._infos = msg['params'][0]

    def is_valid(self):
        return self._valid

    def get_begin_ts(self):
        return self._infos['traces-begin-ts']

    def get_end_ts(self):
        return self._infos['traces-end-ts']

    def get_cur_ts(self):
        return self._infos['traces-cur-ts']

    def get_processed_events(self):
        return self._infos['processed-events']

    def get_state_changes(self):
        return self._infos['state-changes']

    def get_traces_paths(self):
        return self._infos['traces-paths']

    def get_state_providers_paths(self):
        return self._infos['state-providers-paths']

    def is_done(self):
        return self.get_cur_ts() == self.get_end_ts()


class QUpdateListener(Qt.QObject):
    update_available = QtCore.pyqtSignal(object)
    zmq_error = QtCore.pyqtSignal()

    def __init__(self, addr, quit_after):
        super().__init__()

        self._addr = addr
        self._quit_after = quit_after
        self._stop = False

    def start(self):
        logger.info('Starting update listener with address {}'.format(self._addr))

        try:
            self._zmq_context = zmq.Context(1)
            self._zmq_socket = self._zmq_context.socket(zmq.SUB)
            self._zmq_socket.setsockopt(zmq.SUBSCRIBE, bytes())
            self._zmq_socket.setsockopt(zmq.RCVTIMEO, 500)
            self._zmq_socket.connect(self._addr)
        except:
            self.zmq_error.emit()
            return

        while not self._stop:
            try:
                msg = self._zmq_socket.recv()

                logger.debug('Received a message from publisher')

                if not self._stop:
                    progress_update = ProgressUpdate(msg)
                    self.update_available.emit(progress_update)
            except:
                logger.debug('Nothing available from publisher')

        logger.info('Update listener stopped')

        self._zmq_context.destroy()

    def stop(self):
        logger.debug('Stopping update listener'.format(self._addr))
        self._stop = True
