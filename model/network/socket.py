import socket
import threading
import traceback

from network.message_history import MessageHistory

class SocketConnector:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((host, port))
        self.lock = threading.Lock()
        self.buffer = ""
        self.listener = None
        self.executor = None
        self.history = MessageHistory()
        self.alive = True

    def set_listener(self, listener):
        self.listener = listener

    def set_executor(self, executor):
        self.executor = executor

    def send_message(self, message, has_response = True):
        msgid = -1
        with self.lock:
            if has_response:
                msgid = self.history.add_message(message)
            self.client_socket.send((message + '\n').encode('utf-8'))
        return msgid

    def receive_messages(self):
        while self.alive:
            try:
                data = self.client_socket.recv(4096).decode('utf-8')
                if data:
                    with self.lock:
                        self.buffer += data
                        self.process_buffer()
                    if self.executor:
                        self.executor()
                else:
                    break
            except Exception as e:
                print(f"Error receiving data: {e}")
                break

    def __is_response(self, message):
        if message == "WELCOME":
            return False
        elif message == "dead":
            return False
        elif message.startswith("Current level"):
            return False
        elif message.startswith("message "):
            return False
        return True

    def process_buffer(self):
        while "\n" in self.buffer:
            message, self.buffer = self.buffer.split("\n", 1)
            self.process_message(message.strip())

    def process_message(self, message):
        if self.listener:
            try:
                if self.__is_response(message):
                    self.history.add_response(message)
                self.listener(message)
            except Exception as e:
                print(traceback.format_exc())
                print(f"Error processing message: {e}")
                print(self.history.history)
                exit()

    def close(self):
        self.client_socket.close()