#!/bin/sh
arduino-cli upload -v -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328old "$1:MecasMicro"
