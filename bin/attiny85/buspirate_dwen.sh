#!/bin/bash
#
# 9F high fuse = DWEN + SPIEN - although SPIEN is auto-disabled while DWEN is enabled, I guess it's OK to have it still programmed as enabled
PIO=~/.platformio/packages/tool-avrdude
AVRDUDE=$PIO/avrdude
CONF="-C $PIO/avrdude.conf"

$AVRDUDE $CONF -c buspirate -P $1 -p attiny85 -v -U hfuse:w:0x9f:m

