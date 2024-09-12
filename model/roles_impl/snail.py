import random
from typing import Callable, Dict
from robo_data import RoboData
from roles_impl.role_base import RoleBase
from handlers.handlers import handle_inventory, handle_look
from robot import Robot

MINERALS = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

class Snail(RoleBase):
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        super().__init__(robot, robo_data, role_setter)
        self.robot = robot
        self.x, self.y = self.robo_data.get_capped_dimensions(max_cap = 10)
        self.segment = 0
        self.current = 0
        self.segment_size = 0


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
        self.robot.take("food", wait=True)
        return self.move_back_to_origin(place, space, can_forward)


    def initial_forward(self):
        count = self.robo_data.get_item("food")
        if count < 20:
            self.take_food_until_inventory_is(20)
        self.segment_size = (self.x // 2) * 2
        for i in range(self.segment_size // 2):
            self.robot.forward()
        self.robot.left(wait=True)
        self.segment = 0
        self.current = 0

    def back_to_base(self):
        for i in range(self.segment_size // 2):
            self.robot.forward()
        for mineral in MINERALS:
            count = self.robo_data.get_item(mineral)
            for i in range(count):
                self.robot.set(mineral, wait=True)
        self.segment = 0
        self.current = 0
        self.segment_size = 0
        self.robot.right()
        self.robot.right(wait=True)


    def comportement(self):
        if self.segment_size == 0:
            self.initial_forward()
        dest_size = self.segment_size
        has_forwarded = self.check_and_take_minerals(can_forward=self.current != dest_size - 1)
        if not has_forwarded:
            self.robot.forward()
        self.current += 1
        if self.segment == 0 or self.segment == 4:
            dest_size //= 2
        if self.current == dest_size:
            self.robot.left(wait=True)
            self.current = 0
            if self.segment == 4:
                self.back_to_base()
            self.segment += 1




    def handle_broadcast(self, message: str, distance: int):
        pass


