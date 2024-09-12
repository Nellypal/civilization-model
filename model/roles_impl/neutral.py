from roles_impl.role_base import RoleBase

class Neutral(RoleBase):
    def comportement(self):
        pass

    def handle_broadcast(self, message: str, distance: int):
        pass
