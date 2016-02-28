# fuse of E2 sets 8mhz clock, otherwise it's 8 / 1
PIO=~/.platformio/packages/tool-avrdude
AVRDUDE=$PIO/avrdude
CONF="-C $PIO/avrdude.conf"

$AVRDUDE $CONF -c buspirate -P $1 -p attiny85 -v -U flash:w:.pioenvs/attiny/firmware.hex -U lfuse:w:0xe2:m
