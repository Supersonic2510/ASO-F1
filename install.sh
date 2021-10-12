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
	echo "LED module has succsefully been installed!"
else
	#statements
	echo "LED module has already been installed!"
fi