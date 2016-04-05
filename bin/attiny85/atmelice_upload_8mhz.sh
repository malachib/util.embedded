#!/bin/bash
#
# fuse of E2 sets 8mhz clock, otherwise it's 8 / 1.  Also using DWEN (debugwire)
# old non-DWEN would be hfuse:w:0xdf:m
# new with-DWEN is      hfuse:w:0xbf:m
# see here for more info http://www.engbedded.com/fusecalc/
#
# NOTE: This script has not worked yet
PIO=~/.platformio/packages/tool-avrdude
#AVRDUDE=$PIO/avrdude
#using global AVRDUDE because PIO one isn't new enough to support Atmel-ICE
AVRDUDE=avrdude
#CONF="-C $PIO/avrdude.conf"
#PROGRAMMER="-c jtag2isp"
#PROGRAMMER="-c atmelice_isp"
PROGRAMMER="-c atmelice_dw"

$AVRDUDE $CONF $PROGRAMMER -p attiny85 -v -U flash:w:.pioenvs/attiny/firmware.hex -U lfuse:w:0xe2:m -U hfuse:w:0xbf:m
