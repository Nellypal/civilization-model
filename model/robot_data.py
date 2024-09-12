import random
from typing import Dict
import uuid


class RobotData:
    def __init__(self):
        self.inventory = {}
        self.broadcasts = {}
        self.id = str(random.randint(11111,99999))
        self.dimensions = (0, 0)
        self.level = 1
        self.cipher = None

    def set_broadcast_distance(self, message: str, distance: int):
        self.broadcasts[message] = distance

    def has_heard_message(self, message: str):
        return message in self.broadcasts

    def get_broadcast_distance(self, message: str):
        return self.broadcasts[message]

    def remove_broadcast_message(self, message: str):
        del self.broadcasts[message]

    def set_inventory(self, inventory: Dict):
        self.inventory = inventory

    def get_inventory(self):
        return self.inventory

    def get_item(self, item):
        return self.inventory.get(item, 0)

    def set_dimensions(self, dimensions: tuple):
        self.dimensions = dimensions

    def get_dimensions(self):
        return self.dimensions

    def get_capped_dimensions(self, max_cap = 10):
        return (min(self.dimensions[0], max_cap), min(self.dimensions[1], max_cap))

    def set_level(self, level: int):
        self.level = level

    def get_level(self):
        return self.level

    def set_cipher(self, cipher: str):
        self.cipher = cipher

    def get_cipher(self):
        return self.cipher