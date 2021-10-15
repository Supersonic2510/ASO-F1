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
	#Move pushbutton 1
	sudo mv /usr/lib/lkm/pushbutton1.sh ./
	#Move pushbutton 2
	sudo mv /usr/lib/lkm/pushbutton2.sh ./
	#Move pushbutton 3
	sudo mv /usr/lib/lkm/pushbutton3.sh ./
	#Move pushbutton 4
	sudo mv /usr/lib/lkm/pushbutton4.sh ./
	sudo rm -rf /usr/lib/lkm
	echo "LED module has succsefully been uninstalled!"
else
	#statements
	echo "LED module has already been uninstalled!"
fi