from typing import Callable
from robot import Robot
from robo_data import RoboData
from handlers.handlers import handle_inventory, handle_look
from roles_impl.role_base import RoleBase


class Survival(RoleBase):
    def __init__(self, robot: Robot, robo_data: RoboData, role_setter: Callable):
        super().__init__(robot, robo_data, role_setter)
        self.forward_counts = 0
        self.displacement = (0, 0)

    def check_and_take_food(self):
        look_result = self.robot.look()
        look_data = handle_look(look_result)
        if not 'food' in look_data:
            return False
        food_places = sorted(look_data['food'], key = lambda x: x["count"])
        best_place = food_places[-1]
        place, space = best_place["place"]
        self.move_to_location(place, space)
        for i in range(min(best_place["count"], 5)):
            self.robot.take('food', wait=True)
        return self.move_back_to_origin(place, space, can_forward=False)

    def comportement(self):
        x, y = self.robo_data.get_capped_dimensions()
        has_forwarded = self.check_and_take_food()
        # if not has_forwarded:
        #     self.robot.forward()
        # self.forward_counts += 1
        # self.displacement = (self.displacement[0] + 1, self.displacement[1])
        # if self.forward_counts > x - 1:
        #     self.robot.right(wait=True)
        #     self.robot.forward()
        #     self.robot.forward()
        #     self.displacement = (self.displacement[0], self.displacement[1] + 2)
        #     self.robot.left(wait=True)
        #     self.forward_counts = 0

    def compensate(self):
        pass
        # x, y = self.robo_data.get_capped_dimensions()
        # self.displacement = (self.displacement[0] % x, self.displacement[1] % y)
        # self.robot.right()
        # self.robot.right(wait=True)
        # for i in range(self.displacement[0]):
        #     self.robot.forward()
        # self.robot.right(wait=True)
        # for i in range(self.displacement[1]):
        #     self.robot.forward()
        # self.robot.right(wait=True)
        # self.forward_counts = 0
        # self.displacement = (0, 0)

    def handle_broadcast(self, message: str, distance: int):
        pass


