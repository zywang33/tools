#!/bin/sh

cc -g -O3 -DUSE_OPENSSL -Wall make_fw.c -o make_firmware

#./make_firmware iram0.data dram0.data fw.bin
