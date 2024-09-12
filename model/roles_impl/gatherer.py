import time
from typing import Callable, Dict
from robot import Robot
from robo_data import RoboData
from handlers.handlers import handle_inventory, handle_look
from roles_impl.role_base import RoleBase

MINERALS = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
RIGHTS_IT = [1, 5, 9, 13]
BACK_TO_BASES = [3, 7, 11, 15]
class Gatherer(RoleBase):
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        super().__init__(robot, robo_data, role_setter)
        self.displacement_step = 0
        self.has_forwarded = False

    def get_best_mineral_place(self, look_data: Dict):
        place_dict = {}

        for item, places in look_data.items():
            if item not in MINERALS:
                continue
            for place in places:
                key = str(place["place"])
                place_count = place["count"]
                if key not in place_dict:
                    place_dict[key] = []
                place_dict[key].append((item, place_count))

        place_values = place_dict.items()
        if len(place_values) == 0:
            return None
        best_places = sorted(place_values, key=lambda x: sum(place[1] for place in x[1]))
        pos = tuple(map(int, best_places[-1][0].strip('()').split(', ')))
        return (pos, best_places[-1][1])

    def check_and_take_minerals(self, can_forward=True):
        look_res = self.robot.look()
        look_data = handle_look(look_res)
        best_place = self.get_best_mineral_place(look_data)
        if not best_place:
            return
        (place, space), items = best_place
        self.move_to_location(place, space)
        for item in items:
            for i in range(item[1]):
                self.robot.take(item[0], wait=True)
        return self.move_back_to_origin(place, space, can_forward)

    def gatherer_back_to_base(self, stay_in_base=False):
        self.robot.right(wait=True)
        self.robot.forward()
        self.robot.forward()
        inventory_data = self.robot.inventory()
        self.robo_data.set_inventory(handle_inventory(inventory_data))
        for mineral in MINERALS:
            count = self.robo_data.get_item(mineral)
            for i in range(count):
                self.robot.set(mineral, wait=True)
        self.robot.right()
        self.robot.right(wait=True)
        if not stay_in_base:
            self.robot.forward()
            self.robot.forward()
            self.robot.right(wait=True)
            return False
        return True

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
            skip_end = self.gatherer_back_to_base(stay_in_base=self.displacement_step == 15)
            if skip_end:
                self.next_displacement()
                return
        self.has_forwarded = self.check_and_take_minerals(can_forward=self.displacement_step not in RIGHTS_IT)
        if self.displacement_step in RIGHTS_IT:
            self.robot.right(wait=True)
        self.next_displacement()

    def handle_broadcast(self, message: str, distance: int):
        pass

