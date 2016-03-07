#!/bin/bash
#
PIO=~/.platformio/packages/tool-bossac
BOSSAC=$PIO/bossac
FIRMWARE_PATH=$2
DEVICE=$1

if [[ $DEVICE == /dev/* ]]; then
  echo "Error: drop the prepending '/dev/' from your device parameter"
fi

# good reference http://serverfault.com/questions/7503/how-to-determine-if-a-bash-variable-is-empty
#if 2nd parameter is set to a non-empty string
if [ -z "${FIRMWARE_PATH}" ]; then
#  echo "Using TEST path: $2"
#else
  FIRMWARE_PATH=adafruit_m0
fi

echo "Using firmware path: $FIRMWARE_PATH"

$BOSSAC --info --port $DEVICE --erase --write --verify --reset --debug .pioenvs/$FIRMWARE_PATH/firmware.bin
