from typing import Dict
from roles_impl.role_base import RoleBase
from handlers.handlers import handle_inventory, handle_look
from robot import Robot

class Poule(RoleBase):
    def comportement(self):
        while True:
            self.robot.fork(wait=True)

    def handle_broadcast(self, message: str, distance: int):
        pass