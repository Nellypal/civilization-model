from roles_impl.role_base import RoleBase

class Parrot(RoleBase):
    def comportement(self):
        self.robot.forward()
        pass

    def handle_broadcast(self, message: str, distance: int):
        if message.startswith("A:") or message.startswith("R:") or message.startswith("L:") or message.startswith("B:"):
            return
        for i in range(3):
            self.robot.broadcast(message, wait=True)
        pass
