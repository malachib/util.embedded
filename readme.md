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

Code is Arduino, POSIX and mbed OS friendly.  Experimental support for ESP-OPEN-RTOS.
Plays nice with PlatformIO

* C++ iostream shims; use istream/ostream etc. without all the bloat
* Circular buffer
* Debug console: Menuing system for serial port (or other stream) control code to run diagnostics and acquire status
    * Telnet capable (coming soon)
* Linked lists, single and doubly linked

Note Circular Buffer and its close relatives conform to the layer1-layer5 design
as described later.  This could be critical for your small-footprint environment.

Experimental:
-------------

Following code has worked to varying degrees but has not been tested for a while.
Code in this category is more subject to breaking changes:

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

Obsolete:
---------

Following code worked at once time, but is now being put out to pasture.  If you
seee it, don't use it:

* Cross-thread RPC mechanism (if using an external threading tool)


Other preprocessor defines
--------------------------

* FEATURE_IOSTREAM: configures POSIX iostreams to take precedence over our lightweight libs

## Code paradigm:

* layer 1: barest-metal, tends to be templates; buffer pointers avoided
* layer 2: low-level.  buffer pointers often used.  size fields are templatized constants
* layer 3: low-level.  buffer pointers and size fields used
* layer 4: mid-level.  buffers inline (pointers avoided).  virtual functions allowed
  * layer 4 is currently experimental and subject to change definition.
* layer 5: mid-level.  buffer pointers used.  virtual functions allowed

Hardware Compatibility
----------------------

Tested and works on the following:

Device           | Framework          | Features
---------------- | ------------------ | --------
ATmega328P       | Arduino            |
ATmega32u4       | Arduino            | IOS, buffer
ATtiny85         | Arduino            |
ESP8266          | Arduino            |
ESP8266          | ESP-OPEN-RTOS      |
ATSAMD21G        | Arduino            | IOS
ATSAMD21G        | mbed OS            |
STM32F401RE      | mbed OS            | IOS
x86              | POSIX              | IOS

Most code in this lib is designed to work everywhere, so missing
features on this chart only represents that it is *untested* , not
necessarily *unavailable*.

Data/Memory code size
---------------------

I won't lie, these libraries take space.  There are a slew of #defines to help curb its appetite:

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
* EVENT_FEATURE_TYPE2: Force events to never operate in Type1 mode, only Type2.  
   *May* save code space if using events heavily.

Notes
-----

Remember, istream/ostream are (arguably) by design blocking I/O.  There is a lot of
discussion about this on the internet.  So far I have done nothing to adjust or enhance
the FEATURE_IOSTREAM_SHIM to be async.
