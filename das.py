import importlib
import sys
import os
import traceback
import time
from os.path import basename
from threading import Event, Thread

import keyboard_api


class Backchannel:
    def write(self, s):
        keyboard_api.log(s)

    def flush(self):
        pass


sys.stdout = Backchannel()
sys.stderr = Backchannel()

print('started at', time.ctime(time.time()))

def once():
    def decorate(f):
        def stub(*args):
            def noop():
                pass

            f(*args)
            return noop
        return stub
    return decorate

def every(minutes=0, seconds=0):
    def decorate(f):
        def stub(*args):
            stopped = Event()

            def loop():
                while not stopped.wait(minutes * 60 + seconds):
                    f(*args)

            Thread(target=loop).start()
            f(*args)
            return stopped.set
        return stub
    return decorate


def set_key_color(pid, key, fx, r, g, b):
    return keyboard_api.set_color(pid, key, fx, r, g, b)


def reset_key_color(pid):
    return keyboard_api.reset_color(pid)


class Plugin:
    def run(self):
        pass


from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler


class Watcher:
    DIRECTORY_TO_WATCH = "./plugins"

    Plugins = {}

    def __init__(self):
        self.observer = Observer()
        for file in os.listdir("./plugins"):
            if file.endswith(".py"):
                Watcher.loadPlugin(os.path.splitext(file)[0])

    def run(self):
        event_handler = Handler()
        self.observer.schedule(event_handler, self.DIRECTORY_TO_WATCH, recursive=True)
        self.observer.start()
        try:
            while True:
                time.sleep(5)
        except:
            self.observer.stop()
            print("Error")

        self.observer.join()

    @staticmethod
    def loadPlugin(plugname):
        fullname = "plugins." + plugname
        try:

            if fullname in sys.modules:
                Watcher.Plugins[fullname]()
                del Watcher.Plugins[fullname]
                del sys.modules[fullname]
                print("reloaded")

            module = importlib.import_module(fullname)

            for t in module.__dict__.values():
                if not isinstance(t, type):
                    continue

                if issubclass(t, Plugin) and not t == Plugin:
                    print("found plugin", t.__name__)
                    tt = t()
                    x = tt.run()
                    Watcher.Plugins[fullname] = x

        except KeyError:
            print("Module {} not loaded. Can't reload".format(plugname))
        except:
            print("Unexpected error:", sys.exc_info()[0])


class Handler(FileSystemEventHandler):

    @staticmethod
    def on_any_event(event):
        if event.is_directory:
            return None

        elif event.event_type == 'created':
            if event.src_path.endswith(".py"):
                print("Received created event - %s." % event.src_path)
                Watcher.loadPlugin(os.path.splitext(basename(event.src_path))[0])

        elif event.event_type == 'modified':
            if event.src_path.endswith(".py"):
                print("Received modified event - %s." % event.src_path)
                Watcher.loadPlugin(os.path.splitext(basename(event.src_path))[0])


w = Watcher()
w.run()
