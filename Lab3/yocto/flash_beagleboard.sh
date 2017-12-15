#!/bin/bash

rm -rf /media/ezamoraa/boot/*
rm -rf /media/ezamoraa/root/*

cp MLO u-boot.img uEnv.txt /media/ezamoraa/boot/
tar x -C /media/ezamoraa/root/ -f radio-car-image-beagleboard.tar.bz2
tar x -C /media/ezamoraa/root/ -f modules-beagleboard.tgz

mkimage -A arm -O linux -T kernel -C none -a 80300000 -e 80300000 -n "Radio Car Image" -d zImage uImage
cp uImage /media/ezamoraa/root/boot/
