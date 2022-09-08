#!/bin/sh
# This script runs the `rpi_script.sh` on the RPi through SSH

user="user"
host="raspberrypi.local"

echo "Sending the project into the home directory of ${user}@${host}..."
./rpi_sync.sh

echo "Connecting to RPi via SSH..."
ssh "${user}@${host}" 'bash -s' < rpi_script.sh
