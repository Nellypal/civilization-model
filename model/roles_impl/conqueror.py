import random
from typing import Callable, Dict
from robo_data import RoboData
from roles_impl.role_base import RoleBase
from handlers.handlers import handle_inventory, handle_look
from robot import Robot

MINERALS = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

class Conqueror(RoleBase):
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        super().__init__(robot, robo_data, role_setter)
        self.robot = robot
        self.x, self.y = self.robo_data.get_capped_dimensions(max_cap = random.randint(5, 10))
        self.current_y = 0
        self.current_x = 0
        self.desaxage = 0


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
        self.robot.forward()
        self.robot.forward()
        self.robot.left(wait=True)
        self.check_and_take_minerals(can_forward=False)


    def comportement(self):
        if self.current_y == 0:
            self.initial_forward()
            self.current_y += 2

        self.robot.forward()
        if self.y - self.current_y > 2:
            self.check_and_take_minerals(can_forward=False)
        self.current_x += 1

        if self.current_x == self.x - 2:
            last_turn = self.current_y + 4 > self.y
            if not last_turn:
                self.robot.right(wait=True)
                self.robot.forward()
                self.robot.forward()
                self.robot.forward()
                self.robot.left(wait=True)
                self.current_x = 0
                self.desaxage += 2
                self.current_y += 3
            else:
                distance = self.y - self.current_y
                self.robot.right(wait=True)
                for i in range(distance):
                   self.robot.forward()
                self.robot.right(wait=True)
                for i in range(self.desaxage):
                   self.robot.forward()
                self.robot.left()
                self.robot.left(wait=True)
                for mineral in MINERALS + ["food"]:
                    count = self.robo_data.get_item(mineral)
                    for i in range(count):
                        self.robot.set(mineral, wait=True)
                self.desaxage = 0
                self.current_y = 0
                self.current_x = 0

    def handle_broadcast(self, message: str, distance: int):
        pass


