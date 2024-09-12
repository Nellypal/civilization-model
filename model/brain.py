import threading
import random
import time
from uuid import uuid4

from roles_impl.snail import Snail
from roles_impl.poule import Poule
from roles_impl.parrot import Parrot
from cipher import decipher_algo
from roles_impl.seeker import Seeker
from roles_impl.leader import Leader
from roles_impl.neutral import Neutral
from roles_impl.concubine import Concubine
from roles_impl.court import Court
from roles_impl.gatherer import Gatherer
from roles_impl.hunter import Hunter
from roles_impl.mathyr import Marthyr
from roles_impl.conqueror import Conqueror
from roles_impl.survival import Survival
from robo_data import RoboData
from handlers.handlers import handle_client_num, handle_inventory
from robot import Robot
import re

ROLES_MAP = {
    "conqueror": Conqueror,
    "gatherer": Gatherer,
    "concubine": Concubine,
    "marthyr": Marthyr,
    "hunter": Hunter,
    "court": Court,
    "parrot": Parrot,
    "poule": Poule,
    "snail": Snail
}
class Brain:
    def __init__(self, robot: Robot):
        self.robot = robot
        self.is_welcomed = False
        self.role = None
        self.is_survival = False
        self.robo_data = RoboData()
        self.survival_mode = Survival(self.robot, self.robo_data, self.set_role)
        self.is_leader = False
        self.is_dead = False

        self.available_slots = 0
        self.checked_available_slots = False
        self.heard_first_broadcast = False

        self.robot.events.on("welcome", self.handle_welcome)
        self.robot.events.on("dimensions", self.handle_dimensions)
        self.robot.events.on("broadcast", self.handle_broadcast)
        self.robot.events.on("dead", self.handle_broadcast)
        self.robot.events.on("level", self.handle_level)

        self.robot.start_listening()

    def get_available_slots(self):
        if self.checked_available_slots:
            return 0
        self.checked_available_slots = True
        return self.available_slots

    def handle_death(self, message: str):
        self.is_dead = True
        self.robot.kill()

    def handle_dimensions(self, message: str):
       self.is_welcomed = True
       parts = message.split()
       if len(parts) == 2:
            dimensions_tuple = (int(parts[0]), int(parts[1]))
            self.robo_data.set_dimensions(dimensions_tuple)

    def handle_welcome(self, _):
       self.robot.send_team()

    def handle_broadcast(self, raw):
        matches = re.search(r"^message (\d+), (.*)$", raw)
        if not matches:
            return
        distance, message = matches.groups()

        cipher_key = self.robo_data.get_cipher()
        if message.startswith("C:") and cipher_key:
            message = decipher_algo(message, cipher_key)
        self.robo_data.set_broadcast_distance(message, int(distance))
        if message.startswith("A:"):
            data = message.split(":")
            if data[1] == self.robo_data.id:
                self.set_role(data[2])
        elif message.startswith("B:"):
            code = message.split(":")[1]
            self.robo_data.set_cipher(code)
            self.robot.cipher_key = code
            self.heard_first_broadcast = True
        if self.role:
            self.role.handle_broadcast(message, int(distance))

    def handle_level(self, raw):
        data = raw.split(":")
        self.robo_data.set_level(int(data[1]))

    def set_role(self, name: str | None):
        if name == None:
            self.role = None
            return
        if name in ROLES_MAP:
            self.role = ROLES_MAP[name](self.robot, self.robo_data, self.set_role)
        else:
            self.role = Neutral(self.robot, self.robo_data, self.set_role)

    def __routine(self):
        while True:
            if self.is_welcomed:
                break

        inventory_data = self.robot.inventory()
        self.robo_data.set_inventory(handle_inventory(inventory_data))
        last_check = time.time()

        while not self.is_dead:
            try:
                if self.robo_data.get_item("food") < 4 and not self.is_survival and not self.is_leader:
                    self.is_survival = True
                if self.robo_data.get_item("food") > 6 and self.is_survival:
                    self.survival_mode.compensate()
                    self.is_survival = False
                if self.is_survival:
                    self.survival_mode.comportement()
                elif not self.role:
                    self.check_if_first()
                else:
                    self.role.comportement()
                if time.time() - last_check > 1:
                    last_check = time.time()
                    connect_nbr_data = self.robot.connect_nbr()
                    self.available_slots = handle_client_num(connect_nbr_data)
                    self.checked_available_slots = False
                    inventory_data = self.robot.inventory()
                    self.robo_data.set_inventory(handle_inventory(inventory_data))
            except Exception as e:
                print("Ignoring", e)

    def run(self):
        routine_thread = threading.Thread(target=self.__routine)
        routine_thread.start()

        self.robot.receive_thread.join()
        routine_thread.join()


    def check_if_first(self):
        for _ in range(8):
            self.robot.left(wait=True)
            time.sleep(0.2)

        if not self.heard_first_broadcast:
            self.robo_data.set_cipher(str(uuid4()).replace("-" , ""))
            self.robot.cipher_key = self.robo_data.get_cipher()
            self.role = Leader(self.robot, self.robo_data, self.set_role)
            self.is_leader = True
        else:
            self.role = Seeker(self.robot, self.robo_data, self.set_role)





