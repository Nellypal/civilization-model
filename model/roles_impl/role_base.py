from typing import Callable
from handlers.handlers import handle_inventory
from robo_data import RoboData
from robot import Robot


class RoleBase:
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        self.robot = robot
        self.robo_data = robo_data
        self.set_role = role_setter

    def move_to_location(self, place, space):
        if place == 0 and space == 0:
            pass
        elif place == 0 and space == 1:
            self.robot.forward()
            self.robot.left(wait=True)
            self.robot.forward()
        elif place == 1 and space == 1:
            self.robot.forward()
        elif place == 2 and space == 1:
            self.robot.forward()
            self.robot.right(wait=True)
            self.robot.forward()

    def take_food_until_inventory_is(self, target_amount: int):
        inventory_data = self.robot.inventory()
        self.robo_data.set_inventory(handle_inventory(inventory_data))
        current_food = self.robo_data.get_item("food")
        for i in range(target_amount - current_food):
            self.robot.take("food", wait=True)

    def move_back_to_origin(self, place, space, can_forward = True):
        if place == 0 and space == 0:
            return False
        elif place == 0 and space == 1:
            self.robot.right()
            self.robot.right(wait=True)
            self.robot.forward()
            if can_forward:
                self.robot.left(wait=True)
                return True
            self.robot.right(wait=True)
            self.robot.forward()
            self.robot.left()
            self.robot.left(wait=True)
            return False
        elif place == 1 and space == 1:
            if can_forward:
                return True
            self.robot.right()
            self.robot.right(wait=True)
            self.robot.forward()
            self.robot.right()
            self.robot.right(wait=True)
            return False
        elif place == 2 and space == 1:
            self.robot.right()
            self.robot.right(wait=True)
            self.robot.forward()
            if can_forward:
                self.robot.right(wait=True)
                return True
            self.robot.left(wait=True)
            self.robot.forward()
            self.robot.right()
            self.robot.right(wait=True)