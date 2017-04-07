#pragma once

// Only include this from NON arduino environments which desire arduino behavior
// right now works as included from Arduino environments but that behavior is
// deprecated 

// this sets up legacy fact_?streams based on whether we are using "real"
// or our shim iostream, but we always now will be using our ostream
#include "features.h"
#include "c_types.h"

#ifndef FEATURE_IOSTREAM
#include "iostream.h"
#ifndef ARDUINO
// Needed for __FlashStringHelper*
#include "noduino.h"

// iostream includes this already if ARDUINO is specified
#include "streams/ostream_arduino.h"
#endif

#else
#include <iostream>
// Needed for __FlashStringHelper*
#include "noduino.h"

#include "streams/ostream_arduino.h"

#endif
