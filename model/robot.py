import threading
from cipher import cipher_algo
from network.events import EventListener
from network.socket import SocketConnector

class Robot:
    def __init__(self, host: str, port: int, team: str):
        self.host = host
        self.port = port
        self.team = team
        self.__socket = SocketConnector(host, port)
        self.events = EventListener()
        self.__socket.set_listener(self.events.handle_message)
        self.__socket.set_executor(self.events.execute)
        self.receive_thread = threading.Thread(target=self.__socket.receive_messages)
        self.cipher_key: str | None = None

    def start_listening(self):
        self.receive_thread.start()

    def kill(self):
        self.__socket.alive = False
        self.__socket.client_socket.close()
        self.receive_thread.join()

    def send_team(self):
        self.__socket.send_message(self.team, has_response=False)


    def forward(self):
        msgid = self.__socket.send_message("Forward")
        return self.__socket.history.wait_for(msgid)

    def right(self, wait = False):
        msgid = self.__socket.send_message("Right")
        if wait:
            return self.__socket.history.wait_for(msgid)
        else:
            return None


    def left(self, wait = False):
        msgid = self.__socket.send_message("Left")
        if wait:
            return self.__socket.history.wait_for(msgid)
        else:
            return None


    def look(self):
        msgid = self.__socket.send_message("Look")
        return self.__socket.history.wait_for(msgid)

    def inventory(self):
        msgid = self.__socket.send_message("Inventory")
        return self.__socket.history.wait_for(msgid)


    def connect_nbr(self):
        msgid = self.__socket.send_message("Connect_nbr")
        return self.__socket.history.wait_for(msgid)


    def fork(self, wait = False):
        msgid = self.__socket.send_message("Fork")
        if wait:
            return self.__socket.history.wait_for(msgid)
        else:
            return None

    def eject(self):
        self.__socket.send_message("Eject")


    def broadcast(self, message, wait = False, cipher = True):
        if cipher and self.cipher_key:
            message = cipher_algo(message, self.cipher_key)
        msgid = self.__socket.send_message(f"Broadcast {message}")
        if wait:
            return self.__socket.history.wait_for(msgid)
        else:
            return None

    def take(self, object, wait = False):
        msgid = self.__socket.send_message(f"Take {object}")
        if wait:
            return self.__socket.history.wait_for(msgid)
        else:
            return None


    def set(self, object, wait = False):
        msgid = self.__socket.send_message(f"Set {object}")
        if wait:
            return self.__socket.history.wait_for(msgid)
        else:
            return None


    def incantation(self, wait=False):
        self.__socket.send_message("Incantation")