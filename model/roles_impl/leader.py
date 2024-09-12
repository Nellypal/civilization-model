import random
from typing import Dict
from robot import Robot
from robo_data import RoboData
from roles_impl.role_base import RoleBase


ROLES_ONE = {
    "poule": 0.6,#+5
    "marthyr": 0.4,#+10
}

ROLES_TWO = {
    "gatherer": 0.1, #+5
    "court": 0.1,#+5
    "concubine": 0.1,
    "marthyr": 0.6,#+10
    "snail": 0.1#+5
}


ROLES_THREE = {
    # "hunter": 0.08,
    "gatherer": 0.03,
    "court": 0.05,
    "concubine": 0.1,
    "marthyr": 0.8,
    "snail": 0.02
}

class Leader(RoleBase):
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: callable): # type: ignore
        super().__init__(robot, robo_data, role_setter)
        self.forkers = 0
        self.nbr_role = 0
        self.base_message = f"B:{self.robo_data.get_cipher()}"

    def comportement(self):
        count = self.robo_data.get_item("food")
        self.robot.broadcast(self.base_message, wait=True, cipher=False)
        if count < 8:
            for i in range(9 - count):
                self.robot.broadcast(self.base_message, wait=True, cipher=False)
                self.robot.take("food", wait=True)
        if self.forkers < 5:
            self.robot.fork(wait=True)

    def assign_role(self, dest):
        choice = random.random()
        total_proba = 0
        assigned_role = None
        pick_roles = ROLES_ONE
        if self.nbr_role > 30:
            pick_roles = ROLES_TWO
        elif self.nbr_role > 60:
            pick_roles = ROLES_THREE
        for role, proba in pick_roles.items():
            total_proba += proba
            if choice <= total_proba:
                assigned_role = role
                break

        if assigned_role == "court" or assigned_role == "poule" or assigned_role == "concubine":
            self.forkers += 1
        self.robot.broadcast(f"A:{dest}:{assigned_role}")
        self.nbr_role += 1

    def handle_broadcast(self, message: str, distance: int):
        if message.startswith("R"):
            dest = message.split(":")[1]
            self.assign_role(dest)