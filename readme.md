reusable low-level utilities for embedded development

Licensed under GPL v3

Branches:

master: [![Build Status](https://travis-ci.org/malachib/util.embedded.svg?branch=master)](https://travis-ci.org/malachib/util.embedded)

dev: [![Build Status](https://travis-ci.org/malachib/util.embedded.svg?branch=dev)](https://travis-ci.org/malachib/util.embedded)

Code paradigm:

layer 1: barest-metal, tends to be templates; buffer pointers avoided
layer 2: low-level.  buffer pointers often used.  size fields are templatized constants
layer 3: low-level.  buffer pointers and size fields used
layer 4: reserved
layer 5: mid-level.  virtual functions allowed
