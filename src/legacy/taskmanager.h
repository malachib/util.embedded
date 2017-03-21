#ifndef _mal_taskmanager
#define _mal_taskmanager

#include "fact/lib.h"
#include "timinggroup.h"
#include "LinkedList.h"

namespace FactUtilEmbedded
{
  class TaskManager;
  class ScheduleManager;

  class IScheduled : public SinglyLinkedNode
  {
  friend TaskManager;
  protected:
    void schedule(ScheduleManager* sm, uint32_t targetTime);

  public:
    // TODO: perhaps we can phase out isWakeupTime in favor of compareWakeupTime?
    virtual bool isWakeupTime(TimingGroup& tg) = 0;
    virtual void execute() = 0;
    virtual void schedule(ScheduleManager* sm) = 0;
    virtual void begin(TimingGroup& tg) = 0;

    // returns < 0 if wakeup < target time
    // returns 0 if target time == wakeup time
    // returns > 0 if wakeup > target time
    // TODO: give this an "error range" which shall also involve
    // extra lookups during ScheduleManager::execute - to accomodate Special
    // situations where only approx time is known during scheduling
    virtual char compareWakeupTime(uint32_t targetTime) = 0;

#ifdef DEBUG
    virtual void printDebug() {}
#endif
  };

  class IScheduledAbsolute : public IScheduled
  {
  protected:
    uint32_t absoluteWakeupTime;

  public:
    virtual bool isWakeupTime(TimingGroup& tg) override;
    virtual char compareWakeupTime(uint32_t targetTime) override;
    virtual void begin(TimingGroup& tg) override;

#ifdef DEBUG
    virtual void printDebug() override;
#endif
  };


  template <class T>
  class IScheduledWithInterval : //public IScheduled
    public IScheduledAbsolute // eventually change this and make it delta-based off TimingGroup
  {
  protected:
    T wakeupInterval;

  public:
    IScheduledWithInterval() {}
    IScheduledWithInterval(T interval) : wakeupInterval(interval) {}
    
    T getInterval() { return wakeupInterval; }

    virtual void schedule(ScheduleManager* sm) override
    {
      absoluteWakeupTime += getInterval();

      //FIX: having to use "IScheduled::" cuz overloading not operating as expected
      IScheduled::schedule(sm, absoluteWakeupTime);
    }
  };


  class ITask : public SinglyLinkedNode
  {
  friend TaskManager;
    protected:
      virtual void execute() = 0;
  };

  class TaskManager
  {
    SinglyLinkedList tasks;
  public:

    void add(ITask& task);
    void wake(ITask& task);
    void sleep(ITask& task);
    void execute();
  };


  // making things extra-fat with TimingGroup passed in to isWakeupTime for now,
  // just so I have all the options and can widdle them down if needed
  class ScheduleManager
  {
    TimingGroup timingGroup; // hard wiring to millisecond version for now
    SinglyLinkedList listScheduled;

  public:
#ifdef DEBUG
    ScheduleManager(TimingGroup::getCurrentTimeFunc getCurrentTime) : timingGroup(getCurrentTime)
    {

    }

    ScheduleManager() {}
#endif
    IScheduled* findNodeJustBefore(uint32_t targetTime);
    IScheduled* findNodeOnOrJustAfter(uint32_t targetTime);
    void add(IScheduled& scheduled, IScheduled& justAfter);
    void insert(IScheduled& justBefore, IScheduled& scheduled);
    void insertAfter(IScheduled& justBefore, IScheduled& scheduled);
    void append(IScheduled& scheduled);

    void add(IScheduled& scheduled);
    
    // WARNING: untested
    void remove(IScheduled& scheduled);

    void execute();

#ifdef DEBUG
    virtual void printDebug();
#endif

    TimingGroup& getTimingGroup() { return timingGroup; }
  };
}
#endif
