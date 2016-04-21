#!/bin/bash

cp  /usr/data/sopena/pnl/pnl-tp.img  /tmp/
tar -xvJf /usr/data/sopena/pnl/linux-4.2.3.tar.xz -C  /tmp
cp /usr/data/sopena/pnl/linux-config-pnl /tmp/linux-4.2.3/.config
cd /tmp/linux-4.2.3
make -j8
cd ~/M1/PNL/Projet
