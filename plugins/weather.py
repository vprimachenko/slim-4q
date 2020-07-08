import re
import urllib.request

from das import Plugin, every, set_key_color


class WeatherPlugin(Plugin):
    config = {
        "": "KEY_CAPS_LOCK"
    }

    @every(minutes=100)
    def run(self):
        for city, key in self.config.items():
            content = urllib.request.urlopen("http://wttr.in/{}?format=%t".format(city)).read()
            temp = int(re.search(r'\d+', content.decode("utf-8")).group())
            if temp < 5:
                set_key_color("weather", key, "SET", 0, 0, 128)
            elif temp < 15:
                set_key_color("weather", key, "SET", 0, 128, 0)
            elif temp < 20:
                set_key_color("weather", key, "SET", 60, 128, 0)
            elif temp < 25:
                set_key_color("weather", key, "SET", 128, 128, 0)
            else:
                set_key_color("weather", key, "SET", 128, 0, 0)
