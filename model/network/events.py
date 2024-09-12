from typing import Callable
import re
import time

class EventListener:
    def __init__(self) -> None:
        self.__callbacks = {}
        self.__execution_queue = []

    def __execute_callback(self, event: str, message: str):
        if event in self.__callbacks:
            self.__execution_queue.append(lambda: self.__callbacks[event](message))

    def handle_message(self, message: str) -> None:
        if message == "WELCOME":
            self.__execute_callback("welcome", message)
        elif message == "dead":
            self.__execute_callback("dead", message)
        elif re.match(r"^\d+$", message):
            self.__execute_callback("connect_nbr", message)
        elif re.match(r"^\d+ \d+$", message):
            self.__execute_callback("dimensions", message)
        elif re.match(r"^message \d+\, .*$", message):
            self.__execute_callback("broadcast", message)
        elif re.match(r"^\[ (\w+ \d+, )*(\w+ \d+) \]$", message):
            self.__execute_callback("inventory", message)
        elif re.match(r"^\[ (.*) \]$", message):
            self.__execute_callback("look", message)
        elif re.match(r"Current level: \d+", message):
            self.__execute_callback("level", message)


    def execute(self) -> None:
        while len(self.__execution_queue) > 0:
            cb = self.__execution_queue.pop()
            cb()

    def on(self, event: str, cb: Callable[[str], None]):
        self.__callbacks[event] = cb
