#!/usr/bin/env python3
from pyfirmata import Arduino, util

# This script requires the special sketch (Watertics.ino) to be loaded in the Arduino
class ArduinoSensors:
    def __init__(self, port: str):
        self.board = Arduino(port)
        self.it = util.Iterator(self.board)
        self.it.start()
        self.board.analog[0].enable_reporting()
        self.board.analog[1].enable_reporting()

    def read_analog(self, n: int) -> float:
        return self.board.analog[n].read()

    def __del__(self):
        if self.board is not None:
            self.board.analog[0].disable_reporting()
            self.board.analog[1].disable_reporting()
