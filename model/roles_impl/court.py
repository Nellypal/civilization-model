import time
from typing import Callable, Dict
from robo_data import RoboData
from roles_impl.role_base import RoleBase
from handlers.handlers import handle_inventory, handle_look
from robot import Robot

class Court(RoleBase):
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        super().__init__(robot, robo_data, role_setter)
        self.is_lower = False
        self.heard_levels = 0
        self.last_incantation = 0

    def comportement(self):
        self.robot.fork(wait=True)
        self.robot.fork(wait=True)
        level = self.robo_data.get_level()
        count = self.robo_data.get_item("food")
        self.robot.broadcast(f"L:{level}")

        look_res = self.robot.look()
        look_data = handle_look(look_res)
        player_in_base = 0
        for case in look_data.get("player", []):
            if case["place"] == (0, 0):
                player_in_base = case["count"]
        if count < 10:
            self.take_food_until_inventory_is(12)
        inventory_data = self.robot.inventory()
        self.robo_data.set_inventory(handle_inventory(inventory_data))
        cunty = self.robo_data.get_item("food")
        if player_in_base > 15:
            if not self.is_lower and self.heard_levels > 5 and time.time() - self.last_incantation > 25:
                self.robot.broadcast(f"I:{level}")
                self.robot.incantation(wait=True)
                self.last_incantation = time.time()

    def handle_broadcast(self, message: str, distance: int):
        if message.startswith("I:"):
            self.last_incantation = time.time()
            data = message.split(":")
            if int(data[1]) > self.robo_data.get_level():
                self.is_lower = True
            else:
                self.is_lower = False
        if message.startswith("L:"):
            data = message.split(":")
            self.heard_levels += 1
            if int(data[1]) > self.robo_data.get_level():
                self.is_lower = True
            else :
                self.is_lower = False