#!/bin/bash
dfu-util -s 0x08000000 -d 0483:df11 -a 0 -D out/debug/firmware.bin
