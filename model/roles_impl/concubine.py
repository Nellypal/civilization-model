from typing import Dict
from roles_impl.role_base import RoleBase
from handlers.handlers import handle_inventory, handle_look
from robot import Robot

class Concubine(RoleBase):
    def comportement(self):
        count = self.robo_data.get_item("food")
        if count < 10:
            self.take_food_until_inventory_is(12)
        inventory_data = self.robot.inventory()
        self.robo_data.set_inventory(handle_inventory(inventory_data))
        self.robot.fork(wait=True)

    def handle_broadcast(self, message: str, distance: int):
        pass