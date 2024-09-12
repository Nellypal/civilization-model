from typing import Callable
from robot import Robot
from robo_data import RoboData
from handlers.handlers import handle_inventory, handle_look
from roles_impl.role_base import RoleBase

RIGHTS_IT = [1, 5, 9, 13]
BACK_TO_BASES = [3, 7, 11, 15]
class Hunter(RoleBase):

    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        super().__init__(robot, robo_data, role_setter)
        self.displacement_step = 0
        self.has_forwarded = False

    def hunter_back_to_base(self, stay_in_base = False):
        self.robot.right(wait=True)
        self.robot.forward()
        self.robot.forward()
        inventory_data = self.robot.inventory()
        self.robo_data.set_inventory(handle_inventory(inventory_data))
        food = self.robo_data.get_item("food")
        for i in range(food, 5, -1):
            self.robot.set("food", wait=True)
        self.robot.right()
        self.robot.right(wait=True)
        if not stay_in_base:
            self.robot.forward()
            self.robot.forward()
            self.robot.right(wait=True)
            return False
        return True

    def check_and_take_food(self, can_forward = True):
        look_result = self.robot.look()
        look_data = handle_look(look_result)
        if not 'food' in look_data:
            return
        food_places = sorted(look_data['food'], key = lambda x: x["count"])
        best_place = food_places[-1]
        place, space = best_place["place"]
        self.move_to_location(place, space)
        for i in range(best_place["count"]):
            self.robot.take('food', wait=True)
        return self.move_back_to_origin(place, space, can_forward)

    def next_displacement(self):
        self.displacement_step = (self.displacement_step + 1) % 16

    def comportement(self):
        if self.displacement_step == 0:
            self.robot.forward()
            self.robot.forward()
            self.robot.right(wait=True)

        if not self.has_forwarded:
            self.robot.forward()
            self.has_forwarded = False
        if self.displacement_step in BACK_TO_BASES:
            skip_end = self.hunter_back_to_base(stay_in_base=self.displacement_step== 15)
            if skip_end:
                self.next_displacement()
                return
        self.has_forwarded = self.check_and_take_food(can_forward=self.displacement_step not in RIGHTS_IT)
        if self.displacement_step in RIGHTS_IT:
            self.robot.right(wait=True)
        self.next_displacement()

    def handle_broadcast(self, message: str, distance: int):
        pass


