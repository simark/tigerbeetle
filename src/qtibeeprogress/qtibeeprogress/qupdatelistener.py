import zmq
import json
from PyQt5 import Qt
from PyQt5 import QtCore
from qtibeeprogress import utils


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

    def get_trace_begin(self):
        return self._infos['traces-begin-timestamp']

    def get_trace_end(self):
        return self._infos['traces-end-timestamp']

    def get_current_time(self):
        return self._infos['current-timestamp']

    def get_processed_events(self):
        return self._infos['traces-processed-events']

    def get_state_changes(self):
        return self._infos['state-changes']

    def get_traces(self):
        return self._infos['traces']

    def get_state_providers(self):
        return self._infos['state-providers']


class QUpdateListener(Qt.QObject):
    update_available = QtCore.pyqtSignal(object)

    def __init__(self, addr):
        super().__init__()

        self._addr = addr
        self._stop = False

    def start(self):
        self._zmq_context = zmq.Context(1)
        self._zmq_socket = self._zmq_context.socket(zmq.SUB)
        self._zmq_socket.setsockopt(zmq.SUBSCRIBE, bytes())
        self._zmq_socket.setsockopt(zmq.RCVTIMEO, 250)
        if not self._zmq_socket.connect(self._addr):
            return

        while not self._stop:
            msg = self._zmq_socket.recv()
            if len(msg) > 0:
                progress_update = ProgressUpdate(msg)
                self.update_available.emit(progress_update)

        self._zmq_context.destroy()

    def stop(self):
        self._stop = True
