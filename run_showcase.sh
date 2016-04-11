#!/bin/sh
chmod +x speed.sh
chmod +x disable_usb_autosuspend.sh
chmod +x camera.sh

su -c speed.sh root
./disable_usb_autosuspend.sh
./camera.sh
./tdot&
gnome-terminal -e ./info.sh
