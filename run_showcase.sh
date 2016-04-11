#!/bin/sh
su -c /home/ubuntu/tdot/speed.sh root
/home/ubuntu/tdot/disable_usb_autosuspend.sh
/home/ubuntu/tdot/camera.sh
cd /home/ubuntu/tdot/
/home/ubuntu/tdot/tdot&
gnome-terminal -e /home/ubuntu/tdot/info.sh
