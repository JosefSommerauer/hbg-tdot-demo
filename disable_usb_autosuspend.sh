#!/bin/sh

for i in /sys/bus/usb/devices/*/power/level; do
  echo -n "$i: ";
  cat $i;
  echo on > $i;
done
