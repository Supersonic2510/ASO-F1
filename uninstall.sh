#!/bin/bash
echo "Uninstalling LED module..."
lsmod | grep -w LED1
firstmodule=$?
lsmod | grep -w LED2
secondmodule=$?

if [[ $firstmodule -eq 0 ]] || [[ $secondmodule -eq 0 ]]
then
	#statements
	sudo rmmod LED1.ko
	sudo rmmod LED2.ko
	echo "LED module has succsefully been uninstalled!"
else
	#statements
	echo "LED module has already been uninstalled!"
fi