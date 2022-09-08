from w1thermsensor import W1ThermSensor

# Class to interface with the W1 temperature sensor
class TemperatureSensor:
    def __init__(self) -> None:
        self.sensor = W1ThermSensor()

    def read(self) -> float:
        return self.sensor.get_temperature()
