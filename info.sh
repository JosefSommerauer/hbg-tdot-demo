#!/bin/zsh

while true
do
	cd /sys/devices/system/cpu/

	cpu0=$(cat cpu0/cpufreq/scaling_cur_freq)
	cpu1=$(cat cpu1/cpufreq/scaling_cur_freq)
	cpu2=$(cat cpu2/cpufreq/scaling_cur_freq)
	cpu3=$(cat cpu3/cpufreq/scaling_cur_freq)
	gpu=$(cat /sys/kernel/debug/clock/gbus/rate)

	cd /sys/devices/virtual/thermal/
	zone0_name=$(cat thermal_zone0/type | tr ' ' '\n')
	zone1_name=$(cat thermal_zone1/type | tr ' ' '\n')
	zone2_name=$(cat thermal_zone2/type | tr ' ' '\n')
	zone3_name=$(cat thermal_zone3/type | tr ' ' '\n')
	zone4_name=$(cat thermal_zone4/type | tr ' ' '\n')
	zone5_name=$(cat thermal_zone5/type | tr ' ' '\n')

	zone0_temp=$(cat thermal_zone0/temp | tr ' ' '\n')
	zone1_temp=$(cat thermal_zone1/temp | tr ' ' '\n')
	zone2_temp=$(cat thermal_zone2/temp | tr ' ' '\n')
	zone3_temp=$(cat thermal_zone3/temp | tr ' ' '\n')
	zone4_temp=$(cat thermal_zone4/temp | tr ' ' '\n')
	zone5_temp=$(cat thermal_zone5/temp | tr ' ' '\n')
	

	clear
	printf '%80s\n' | tr ' ' =
	echo speed info:
	printf '%80s\n' | tr ' ' -
	echo cpu0: $(($cpu0/1000.0)) MHz
	echo cpu1: $(($cpu1/1000.0)) MHz
	echo cpu2: $(($cpu2/1000.0)) MHz
	echo cpu3: $(($cpu3/1000.0)) MHz
	echo gpu0: $((gpu/1000000.0)) MHz
	printf '%80s\n' | tr ' ' =

	echo
	printf '%80s\n' | tr ' ' =
	echo thermal info:
	printf '%80s\n' | tr ' ' -
	echo $zone0_name: $(($zone0_temp/1000.0))C°
	echo $zone1_name: $(($zone1_temp/1000.0))C°
	echo $zone2_name: $(($zone2_temp/1000.0))C°
	echo $zone3_name: $(($zone3_temp/1000.0))C°
	echo $zone4_name: $(($zone4_temp/1000.0))C°
	echo $zone5_name: $(($zone5_temp/1000.0))C°
	printf '%80s\n' | tr ' ' =
	sleep 0.5
done


