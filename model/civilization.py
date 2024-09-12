#!/usr/bin/env python3
import argparse
import time
import threading
from brain import Brain
from robot import Robot

def parse_args():
    parser = argparse.ArgumentParser(description="Engine IA", add_help=False)
    parser.add_argument("-p", "--port", type=int, required=True, help="Port number")
    parser.add_argument("-n", "--name", required=True, help="Name of the team")
    parser.add_argument("-h", "--host", default="localhost", help="Name of the machine (default: localhost)")
    return parser.parse_args()


def handle_message(message):
    print(message)


def spawn_robo(host, port, name):
    robot = Robot(host, port, name)
    brain = Brain(robot)
    threading.Thread(target=brain.run).start()
    return brain


def get_first_alive_brain(brains: list):
    i = 0
    while brains[i].is_dead:
        i += 1
    return brains[i]


def main():
    args = parse_args()
    brains = []
    brains.append(spawn_robo(args.host, args.port, args.name))

    while True:
        brain = get_first_alive_brain(brains)
        for _ in range(brain.get_available_slots()):
            brains.append(spawn_robo(args.host, args.port, args.name))
            time.sleep(0.05)
        time.sleep(0.5)


if __name__ == "__main__":
    main()
