#!/bin/sh
rsync -urltv -e ssh . user@raspberrypi.local:/home/user/mecas
