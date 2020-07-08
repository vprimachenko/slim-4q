import random

from das import Plugin, every, set_key_color


class PartyPlugin(Plugin):
    @every(seconds=10)
    def run(self):
        r = random.randrange(0, 255, 1)
        g = random.randrange(0, 255, 1)
        b = random.randrange(0, 255, 1)
        set_key_color("party", "KEY_CONTROL_LEFT", "SET", r, g, b)

