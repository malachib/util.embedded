#include "button.h"

namespace FactUtilEmbedded { namespace state {

void Button::update(bool pressed)
{
    if(isPressed())
    {
        if(!pressed)
            state = RELEASED;
        else if(state == PRESSED) state = PRESSING;
    }
    else if(pressed)
    {
        state = PRESSED;
    }
    else // if not pressed, and state was not pressed or pressing, then this is a 2nd non-pressed
        // event (RELEASE) state and resets back to Idle
    {
        state = IDLE;
    }
}

#ifdef ARDUINO
void ButtonWithTimer::update(bool pressed)
{
    Button::update(pressed);
    if(getState() == PRESSED)
    {
        buttonInitialPressTimestamp = millis();
    }
}
#endif

}

}
