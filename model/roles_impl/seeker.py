from roles_impl.role_base import RoleBase


class Seeker(RoleBase):
    def comportement(self):
        base_message = f"B:{self.robo_data.get_cipher()}"

        if not self.robo_data.has_heard_message(base_message):
            return
        direction = self.robo_data.get_broadcast_distance(base_message)
        if direction == 0:
            self.set_role("requester")
            self.robot.broadcast(f"R:{self.robo_data.id}")
        elif direction == 1:
            self.robot.forward()
        elif direction == 2:
            self.robot.left()
            self.robot.forward()
            self.robot.right()
            self.robot.forward()
        elif direction == 3:
            self.robot.left()
            self.robot.forward()
        elif direction == 4:
            self.robot.left()
            self.robot.forward()
            self.robot.left()
            self.robot.forward()
        elif direction == 5:
            self.robot.right()
            self.robot.right()
            self.robot.forward()
        elif direction == 6:
            self.robot.right()
            self.robot.forward()
            self.robot.right()
            self.robot.forward()
        elif direction == 7:
            self.robot.right()
            self.robot.forward()
        elif direction == 8:
            self.robot.right()
            self.robot.forward()
            self.robot.left()
            self.robot.forward()
        self.robo_data.remove_broadcast_message(base_message)

    def handle_broadcast(self, message: str, distance: int):
        pass

