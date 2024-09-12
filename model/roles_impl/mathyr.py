from typing import Dict
from roles_impl.role_base import RoleBase

class Marthyr(RoleBase):
    def comportement(self):
        while True:
            self.robot.set("food", wait=True)

    def handle_broadcast(self, message: str, distance: int):
        pass