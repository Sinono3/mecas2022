# Runs on Raspberry Pi 3
import threading
import time

from flask import Flask, render_template
from arduino import ArduinoSensors
from therm import TemperatureSensor
from RPLCD.i2c import CharLCD

# 'MecasMicro' must be loaded in the Arduino.

# This values will be read by the threads
# These values are placeholder
initialized = False
ph = 7.0
depth = 75.0
temp = 20.0


def sensor_thread():
    global initialized

    print("Initializing sensors...")
    arduino = ArduinoSensors("/dev/ttyUSB0")
    temp_sensor = TemperatureSensor()
    initialized = True
    print("Sensors initialized")

    global ph, depth, temp

    while True:
        ph_analog = arduino.read_analog(0)

        if ph_analog is not None:
            ph = ((1023 - ph_analog) / 73.07) / 2.0
        else:
            print("pH reading failed")

        # Depth is hardcoded for now
        # depth = arduino.read_analog(1)
        temp = temp_sensor.read()

        print(
            f"pH: {ph:.2f}\n"
            + f"Profundidad: {depth:.2f}cm\n"
            + f"Temperatura: {temp:.2f}°C"
        )
        time.sleep(1.0)


def lcd_thread():
    try:
        lcd = CharLCD("PCF8574", 0x27)
    except:
        print("LCD initialization failed")
        quit()

    while True:
        try:
            lcd.clear()
            lcd.cursor_pos = (0, 0)

            if initialized:
                text = (
                    f"pH:\r\n{ph:.2f}\r\n"
                    + f"Profundidad: {depth:.0f}cm\r\n"
                    + f"Temperatura: {temp:.2f}C"
                )
            else:
                text = "\r\n\r\n\r\nInicializando..."

            lcd.write_string(text)
        except:
            print("LCD write failed")

        time.sleep(1.0)


def flask_thread():
    app = Flask(__name__)

    @app.route("/")
    def index():
        return render_template(
            "index.html",
            initialized=initialized,
            ph=f"{ph:.2f}",
            depth=f"{depth:.2f}cm",
            temp=f"{temp:.2f}C",
        )

    app.run(host="0.0.0.0", port=5000, use_reloader=False)


if __name__ == "__main__":
    threading.Thread(target=sensor_thread).start()
    threading.Thread(target=lcd_thread).start()
    threading.Thread(target=flask_thread).start()
