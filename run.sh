#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker,server=/mnt/wslg/PulseServer -machine pcspk-audiodev=speaker