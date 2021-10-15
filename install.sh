#!/bin/bash
echo "Installing LED module..."
lsmod | grep -w LED1
firstmodule=$?
lsmod | grep -w LED2
secondmodule=$?
if [[ $firstmodule -ne 0 ]] || [[ $secondmodule -ne 0 ]]
then
	#statements
	make -s
	sudo insmod LED1.ko
	sudo insmod LED2.ko
	GLOBIGNORE=*.c:Makefile:*.sh:README.md
	rm -rf *
	unset GLOBIGNORE
	rm -rf *.mod.c
	touch output.txt
	sudo mkdir /usr/lib/lkm
	sudo mv output.txt /usr/lib/lkm
	#create scipt pushbutton 1
	sudo chmod +x pushbutton1.sh
	sudo mv pushbutton1.sh /usr/lib/lkm 
	#create scipt pushbutton 2
	sudo chmod +x pushbutton2.sh
	sudo mv pushbutton2.sh /usr/lib/lkm
	#create scipt pushbutton 3
	sudo chmod +x pushbutton3.sh
	sudo mv pushbutton3.sh /usr/lib/lkm
	#create scipt pushbutton 4
	sudo chmod +x pushbutton4.sh
	sudo mv pushbutton4.sh /usr/lib/lkm
	echo "LED module has succsefully been installed!"
else
	#statements
	echo "LED module has already been installed!"
fi