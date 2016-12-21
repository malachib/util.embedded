//#include "lib.h"
#include "fact/statemachine.h"
#include "Console.Stream.h"

//#define DEBUG2

namespace FactUtilEmbedded
{
    using namespace std;

#ifdef DEBUG
  int StateMachineBase::idSeed = 0;
  const PROGMEM char dbgPostProcess[] = "StateMachine::Execute ID = %d, new state = %d";
#endif

  TimingGroup StateMachineBase::globalTimingGroup;

#ifdef DEBUG2
  const int debug_thinning = 2000;
  int debug_thin_current = 0;
#endif

  void StateMachineBase::BaseProcess()
  {
    //timingGroup->currentTime = GetCurrentMilliseconds();
#ifdef DEBUG3
    return;
// careful cuz this throws timing WAY off.
    if(debug_thin_current++ < debug_thinning)
      return;

    debug_thin_current = 0;

    clog
      //<< F("Current state") << currentState
      //<< F(", millis = ") << timingGroup->currentTime
      << F(", lastAction finish = ") << timingGroup->lastActionTimeFinish;
    //char test[128];
    //int msgSize = sizeof(debugMessage2);
    //sprintf(test, "Current state=%d, millis = %ld, lastAction = %ld, interval=%ld", currentState, currentTime, lastAction, currentActionInterval);
    //Serial.println(test);
    clog << endl;
#endif
  }

  void StateMachineBase::PostProcess(int newCurrentState)
  {
    timingGroup->lastActionTimeFinish = timingGroup->GetCurrentTime();
#ifdef ALL_OBSERVABLE
    notify();
#endif
    if(observer != NULL)
      observer->observe(this);
#if DEBUG3
    char temp[80];
    strcpy_P(temp, dbgPostProcess);
    //sprintf(temp, "StateMachine::Execute ID = %d, new state = %d", ID, newCurrentState);
    // FIX: experimental, sprintf into same buffer seems dangerous
    sprintf(temp, temp, ID, newCurrentState);
    clog << temp << endl;
#endif
  }

  byte ToggleStateMachine::Process(byte currentState)
  {
    switch(currentState)
    {
      case On:
        handler.On();

      case OnWaiting:
        if(!IsIntervalReached(interval))
          return OnWaiting;

      case Off:
        handler.Off();

      case OffWaiting:
        if(!IsIntervalReached(interval))
          return OffWaiting;
        return CycleComplete; // Explicitly do this so external parties can inspect the state change

      case CycleComplete:
        return On;

      default:
#ifdef DEBUG
        clog << F("ToggleStateMachine::Process unknown state = ");
        clog << currentState;
        clog << endl;
#endif
        return 255;
    }
  }

  bool SequenceStateMachine::ProcessCycle()
  {
    Execute();
    return currentState == -1;
  }

  int SequenceStateMachine::Process(int state)
  {
    // -1 means we reached the end of a process cycle, so bump forward immediately
    if(state == -1)
      state = 0;

    ICyclingHandler* handler = handlers[state];

    if(handler->ProcessCycle())
    {
#ifdef DEBUG2
      clog << F("SequenceStateMachine::Process state = ") << state;
      clog << endl;
#endif
      if(++state >= stateUpperBound)
        state = -1;

      return state;
      //return currentState != stateUpperBound ? currentState + 1 : 0;
    }
    else
      return state;
  }

  bool ToggleStateMachine::ProcessCycle()
  {
    Execute();
    return currentState == CycleComplete;
  }

  byte IntervalStateMachine::Process(byte state)
  {
    switch(state)
    {
      case Action:
        handler.ProcessCycle();

      case ActionWaiting:
        if(!IsIntervalReached(interval))
          return ActionWaiting;
        return CycleComplete; // Explicitly do this so external parties can inspect the state change

      case CycleComplete:
        return Action;

      default:
#ifdef DEBUG
        clog << F("IntervalStateMachine::Process unknown state = ");
        clog << currentState;
        clog << endl;
#endif
        return 255;

    }
  }

/*
  bool IntervalStateMachine::ProcessCycle()
  {
    Execute();
    return currentState == CycleComplete;
  }



  bool IntervalStateMachine2::ProcessCycle()
  {
    Execute();
    return currentState == CycleComplete;
  }
*/


  byte IntervalStateMachine2::Process(byte state)
  {
    switch(state)
    {
      case Action:
        handler();

      case ActionWaiting:
        if(!IsIntervalReached(interval))
          return ActionWaiting;
        return CycleComplete; // Explicitly do this so external parties can inspect the state change

      case CycleComplete:
        return Action;

        default:
  #ifdef DEBUG
          clog << F("IntervalStateMachine2::Process unknown state = ");
          clog << currentState;
          clog << endl;
  #endif
          return 255;
    }
  }

  int FuncStateMachine::Process(int state)
  {
    vector++;
    vectorHandler.ProcessVector(vector);
	return 0;
  }

  bool AggregateStateMachine::ProcessCycle()
  {
    ICyclingHandler** handler = handlers;

    for(uint8_t i = handlerCount; i > 0; i--)
    {
      (*handler)->ProcessCycle();
      handler++;
    }

	return true;
  }

#ifndef ALL_OBSERVABLE
  byte ObservableStateMachine::Process(byte state)
  {
    byte newState = WrappedStateMachine::Process(state);
    if(newState != state)
      notify();

    return newState;
  }
#endif

#ifdef DEBUG
  byte StateMachineObserverMessenger::getMessage(IObservable* subject)
  {
    StateMachineTemplate<byte>* sm = (StateMachineTemplate<byte>*) subject;
    return sm->getCurrentState();
  }
#endif

}
