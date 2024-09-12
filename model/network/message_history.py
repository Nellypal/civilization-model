import time
import threading

class MessageHistory:
    def __init__(self):
        self.history = []

    def add_message(self, msg):
        self.history.append((msg, None))
        return len(self.history) - 1

    def add_response(self, response):
        if len(self.history) == 0:
            return
        response_index = len(self.history) - 1
        while response_index >= 0 and self.history[response_index][1] == None:
            response_index -= 1
        response_index += 1
        if response_index >= len(self.history):
            return
        if response == "Elevation underway" and self.history[response_index][0] != "Incantation":
            return
        self.history[response_index] = (self.history[response_index][0], response)

    def wait_for(self, index):
        i = 0
        while self.history[index][1] == None:
            i += 1
            # if i > 300:
                # print(self.history[-5:])
            time.sleep(0.01)
            pass
        return self.history[index][1]
