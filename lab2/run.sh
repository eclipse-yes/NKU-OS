#!/usr/bin/bash

wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.19.10.tar.xz
time tar xvJF linux-5.19.10.r.xz

gcc main.c -O2 -o cpdir
trash linux-5.19.10bak
time ./cpdir -d ./linux-5.19.10 | tee log.txt

# Undone work for dir-copy program
# 1. unable to copy link file (only able to copy directory and regular file)
# 2. using too much file handler (unable to execute under ulimit -n 1024)

