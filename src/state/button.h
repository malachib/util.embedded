#pragma once

#include <stdint.h>
#ifndef NODUINO
#include <Arduino.h>
#endif

namespace FactUtilEmbedded {
namespace state {

class Button
{
public:
    enum State
    {
        IDLE,
        PRESSED,  // initial press
        PRESSING, // still pressing
        RELEASED // Released only lasts an instant, which is why we don't use the term "pressed"
    };

    void update(bool pressed);

private:
    State state = IDLE;

public:
    State getState() const { return state; }
    void reset() { state = IDLE; }
    // returns if button is in any form of pressed state
    bool isPressed() const { return state == PRESSED || state == PRESSING; }

protected:
    void setState(State state) { this->state = state; }
};


#ifndef NODUINO
class ButtonWithTimer : public Button
{
    uint32_t buttonInitialPressTimestamp;

public:
    void update(bool pressed);

    uint32_t elapsedSinceInitialPress() const
    {
        return millis() - buttonInitialPressTimestamp;
    }
};
#endif
}
}
