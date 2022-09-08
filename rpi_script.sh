#!/bin/sh

# WARN: Add this to your `/boot/config.txt`:
# dtparam=i2c_arm=on
# dtoverlay=w1-gpio

# Install system dependencies
printf "Installing system dependencies...\n"
sudo apt-get install -y git

# Install PIP dependencies
printf "Installing PIP dependencies...\n"
sudo pip install flask w1thermsensor pyfirmata smbus2 RPLCD

# CD into the directory 
# This should have been synced beforehand in the `rpi_ssh_setup.sh` script.
cd "$HOME/mecas" || exit 1

# TODO: Flash Arduino

# Copy, enable and start system service
service="mecas.service"
printf "Setting up system service...\n"
sudo cp mecas.service "/etc/systemd/system/${service}"
sudo chmod 644 "/etc/systemd/system/${service}"
sudo systemctl daemon-reload
sudo systemctl enable "$service" --now
