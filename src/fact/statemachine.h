/*
#ifndef Arduino_h
#include <Arduino.h>
#endif
*/
#ifndef _mal_statemachine_h
#define _mal_statemachine_h

#include "lib.h"
#include "timinggroup.h"

//#define DEBUG 1

//#define ALL_OBSERVABLE

namespace FactUtilEmbedded
{
  template <class TState>
  class WrappedStateMachine;

  class StateMachineBase  : //: public ICyclingHandler
#ifdef ALL_OBSERVABLE
  // TODO: Make this a "has a" an unionize it,
  // so that we can switch between array, linked list or a message-box approach
  // OR, rather, make this an alternate pattern where only one observer can listen (don't
  // remember that name of that pattern) and then the observer can in turn maintain a list
  // of observers, if desired
      public ObservableArray,
#endif
      public IObservable

  {
/*
    union Notifier
    {
      IObserver* observer;
      ObservableArray observableArray;
      //ObservableLinkedList* observableLinkedList; // this one we're very likely stuck with dynamic allocation, so might as well use pointer here
      // TODO: put the message-box in here
    };*/

    IObserver* observer;

    //Notifier notifier;

    static TimingGroup globalTimingGroup;
protected:
#ifdef DEBUG
    static int idSeed;
    int ID = idSeed++;
#endif

    TimingGroup* timingGroup = &globalTimingGroup;

    StateMachineBase()
    {
      //timingGroup = &globalTimingGroup;
    }

    void BaseProcess();

    // runs only when a new state has been assigned
    void PostProcess(int newCurrentState);

    bool IsIntervalReached(uint32_t interval, bool registerActionTime = true)
    {
      return timingGroup->IsIntervalReached(interval, registerActionTime);
    }

public:
    void SetTimingGroup(TimingGroup* timingGroup)
    {
      this->timingGroup = timingGroup;
    }

#ifdef DEBUG
    virtual
    void printDebugState()
    = 0
    ;
#endif

    void addObserver(IObserver* observer)
    {
      // TODO: add logic to auto-expand to ObserverArray
      this->observer = observer;
    }

	/*
	void observe()
	{
      if(observer != null)
		  observer->observe(this);
	}
	*/
  };

  class AggregateStateMachine : public StateMachineBase, public ICyclingHandler
  {
    static AggregateStateMachine* defaultASM;

    ICyclingHandler** handlers;
    uint8_t handlerCount;

  public:
    bool ProcessCycle() override;

    static AggregateStateMachine* getDefault() { return defaultASM; }
    static void setDefault(AggregateStateMachine* value) { defaultASM = value; }
  };

template <class TState>
class StateMachineTemplate : public StateMachineBase
{
    friend WrappedStateMachine<TState>;

protected:
  //State currentState;
  // TODO: make a non-switch based version of state/delay analysis+response.  Something like
  // a C# IDictionary
  TState currentState;

  virtual TState Process(TState state) = 0;

public:
  // By nature, template functions must be inline/compiler-discoverable
  void Execute()
  {
    BaseProcess();
    TState newCurrentState = Process(currentState);
    if(newCurrentState != currentState)
    {
      PostProcess(newCurrentState);
      currentState = newCurrentState;
    }
  }

#ifdef DEBUG
  TState getCurrentState() { return currentState; }

  void printDebugState() //override
  {
#if AVR
    Serial << F("Current state: ") << getCurrentState();
#endif
  }
#endif
};

template <class TInterval>
class IntervalStateMachineBase :
  public StateMachineTemplate<byte>,
  public ICyclingHandler
{
protected:
  const TInterval interval;

  enum IntervalStates : byte
  {
    Action,
    ActionWaiting,
    CycleComplete
  };

public:
  IntervalStateMachineBase(TInterval _interval) : interval(_interval) {}

  bool ProcessCycle()
  {
    Execute();
    return currentState == CycleComplete;
  }

#ifdef DEBUG2
  bool TESTVAL;

  void SetTestVal(bool _val) { TESTVAL = _val; }
#endif
};

class IntervalStateMachine :
  public IntervalStateMachineBase<uint32_t>
  // TODO: Make this a template with specialization so that we can do 8/16/32 bit versions
{
  ICyclingHandler& handler;

  virtual byte Process(byte state) override;

public:

  IntervalStateMachine(ICyclingHandler& _handler, uint32_t _interval = 1000) :
    IntervalStateMachineBase(_interval),
    handler(_handler)
    {}
};


class IntervalStateMachine2 :
  public IntervalStateMachineBase<uint32_t>
  // TODO: Make this a template with specialization so that we can do 8/16/32 bit versions
{
  void (*handler)(void);

  virtual byte Process(byte state) override;

public:
  IntervalStateMachine2(void (*_handler)(void), uint32_t _interval = 1000) :
    IntervalStateMachineBase(_interval),
    handler(_handler)
    {}
};




// Switches back and forth between On and Off
class ToggleStateMachine :
  public IntervalStateMachineBase<uint16_t>
{
  IToggleHandler& handler;

protected:
  virtual byte Process(byte state) override;

public:
  virtual bool ProcessCycle() override;

  enum ToggleStates : byte
  {
    On,
    OnWaiting,
    Off,
    OffWaiting,
    CycleComplete
  };

  ToggleStateMachine(IToggleHandler& _handler, uint16_t _interval = 1000) :
    IntervalStateMachineBase(_interval),
    handler(_handler) {}
};


class SequenceStateMachine :
  public StateMachineTemplate<int>,
  public ICyclingHandler
{
  // array of handlers ptrs mapping to int state #
  ICyclingHandler** handlers;
  const int stateUpperBound; // FIX: make this assignable

protected:
  virtual int Process(int state) override;

public:
  SequenceStateMachine(ICyclingHandler* _handlers[], int upperBound) :
  handlers(_handlers), stateUpperBound(upperBound) {}

  virtual bool ProcessCycle() override;
};


class FuncStateMachine : public StateMachineTemplate<int>
{
  IVectorHandler& vectorHandler;
  int vector = 0;
  const int begin, end;
  const uint16_t interval;

protected:
  virtual int Process(int state) override;

public:
  FuncStateMachine(IVectorHandler& _vectorHandler, int begin, int end, uint16_t interval) :
    vectorHandler(_vectorHandler), begin(begin), end(end), interval(interval)
  {
  }

};

template <class TState>
class WrappedStateMachine
{
  StateMachineTemplate<TState>* underlyingStateMachine;
protected:
  StateMachineTemplate<TState>* getUnderlyingStateMachine()
  {
    return underlyingStateMachine;
  };

  TState Process(TState state)
  {
    return underlyingStateMachine->Process(state);
  }

  WrappedStateMachine(StateMachineTemplate<TState>* underlyingStateMachine) :
    underlyingStateMachine(underlyingStateMachine)
    {}
};

#ifndef ALL_OBSERVABLE
// hard wiring this to array flavor because ultimately linked list version would
// want dynamic memory allocation
class ObservableStateMachine :
  public WrappedStateMachine<byte>,
  public StateMachineTemplate<byte>,
  //public ICyclingHandler,
  public ObservableArray
{
protected:
  virtual byte Process(byte state) override;

public:
  ObservableStateMachine(
    StateMachineTemplate<byte>* underlyingStateMachine,
    IObserver** observers, byte observerCount)
    :
    ObservableArray(observers, observerCount),
    WrappedStateMachine(underlyingStateMachine)
  {
  }
};
#endif

class StateMachineObserverMessenger : public ObserverMessenger<byte>
{
protected:
#ifdef DEBUG
  byte getMessage(IObservable* observable) override;
#endif

public:
  StateMachineObserverMessenger(byte* messages, int size) :
    ObserverMessenger<byte>(messages, size)
    {}
};

template <class TState>
class StateMachineStatic
{
  static TState state;
protected:
  TState getState() { return state; }
};

}
#endif
