#pragma once

#include <Arduino.h>
//#include <functional>

#include "lib.h"

typedef void (*outputCharFunc)(void* context, char character);

// somewhat hack-y but much lighter weight dtostrf flavor
void dtostrf_func(double value, uint8_t prec, outputCharFunc outputChar, void* context = NULL, bool outputDelimiter = true);
//void dtostrf_func(double value, uint8_t prec, std::function<void (int)> outputChar);
