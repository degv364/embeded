#!/bin/bash
# Script to populate Beagleboard boot files. To be run in build/tmp/deploy/images/beagleboard Yocto Directory

# Asumes MMC/SD partitions (for example mmcblk0p1,mccblk0p2) are mounted
# in /media/"$USER"/boot and /media/"$USER"/root

cp MLO /media/"$USER"/boot/MLO
cp u-boot.img /media/"$USER"/boot/u-boot.img
#First Install u-boot-mkimage package
mkimage -A arm -O linux -T kernel -C gzip -a 80300000 -e 80300000 -n myuImage -d zImage uImage
cp uImage /media/"$USER"/boot/uImage
