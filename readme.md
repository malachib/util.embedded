Util.Embedded
=============

Licensed under GPL v3

Branches: 
master [![Build Status](https://travis-ci.org/malachib/util.embedded.svg?branch=master)](https://travis-ci.org/malachib/util.embedded)
dev [![Build Status](https://travis-ci.org/malachib/util.embedded.svg?branch=dev)](https://travis-ci.org/malachib/util.embedded)

Overview
--------

Comprises reusable low-level functionality for use in your embedded and/or 
cross-platform endeavors.

Code is Arduino and PlatformIO friendly

* Debug console: Menuing system for serial port (or other stream) control code to run diagnostics and acquire status
    * Telnet capable (coming soon)
* Linked lists, single and doubly linked
* Circular buffer
* C#-like event code
* Service objects facilitating start, stop and status querying
 * Canned MQTT Service for use with Adafruit's MQTT libraries (must define either MQTT_TYPE_WIFI or MQTT_TYPE_FONA).  Hard wired to either WiFi or FONA at this time
 * Canned Wifi Service for use with ESP8266 (must define MQTT_TYPE_WIFI)
* Abstract driver layer (work in progress):
 * GNSS (GPS)
* Hardware helper layer - strong effort put to be HAL-like, but no promises =)
    * AVR
        * power management
        * sleep
        * watchdog
        * pin change interrupts
    * SAM
        * sleep (coming soon)
 
Data/Memory code size
---------------------

I won't lie, these libraries take space.  There are a slew of #defines to help curb
its appetite:

* MEMORY_OPT: Trim memory by reducing internal buffers sizes and string descriptions
 * MEMORY_OPT_CODE: Trim code space by reducing size of string descriptions,
   some debug/bounds code, and a few potentially less critical features
 * MEMORY_OPT_DATA: Trim data space by reducing various buffer sizes
* CONSOLE_FEATURE_FULL: Enable all options for Console (enabled by default when 
  MEMORY_OPT_CODE is not enabled)
 * CONSOLE_FEATURE_AUTOCOMPLETE_COMMAND: Enable 'tab' key to auto-complete a menu
   name in console mode
 * CONSOLE_FEATURE_COUT: Presently disabled by default, this switches Console code 
   from global singular-stream to Console managing its own private stream; useful
   for multiple consoles speaking over different streams (i.e. Serial and telnet)
* DRIVER_FEATURE_VTABLE_SUPPRESS: Disables virtual tables/polymorphism for drivers

Other preprocessor defines
--------------------------

(coming soon)


Code paradigm:

* layer 1: barest-metal, tends to be templates; buffer pointers avoided
* layer 2: low-level.  buffer pointers often used.  size fields are templatized constants
* layer 3: low-level.  buffer pointers and size fields used
* layer 4: reserved
* layer 5: mid-level.  virtual functions allowed

Hardware Compatibility
----------------------

Tested and works on the following:

Device               | Features 
-------------------- | --------
ATmega328P           |
ESP8266 12E          |
ATtiny85             |
ATSAMD21G            |
x86                  |
