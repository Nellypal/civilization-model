import re
import time

MINERALS = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

def handle_inventory(raw):
    pattern = r"(\w+)\s+(\d+)"

    matches = re.findall(pattern, raw)

    return {item: int(number) for item, number in matches}

def get_piramidale(case):
    pattern = []
    row = 0
    while len(pattern) < case:
        col_count = row + 1
        for col in range(col_count):
            pattern.append((row, col))
            if len(pattern) == case:
                break
        row += 1
    return pattern


def handle_look(raw):
    raw = raw.strip("[] ")
    cases = raw.split(",")[:4]
    look_count = len(cases)
    case_view = get_piramidale(look_count)
    look_data = {}
    for i, case in enumerate(cases):
        case_place = case_view[i]
        case_items = [i.strip() for i in case.split()]
        for item in case_items:
            if item not in look_data:
                look_data[item] = []
            if any([l["place"] == case_place for l in look_data[item]]):
                continue
            look_data[item].append({
                "count": case_items.count(item),
                "place" : case_place
            })
    return look_data

def handle_client_num(message):
    return int(message)