
#include "taskmanager.h"

namespace FactUtilEmbedded
{
  void TaskManager::sleep(ITask& task)
  {
  }

  void TaskManager::add(ITask& task)
  {
    tasks.add(&task);
  }

  void TaskManager::execute()
  {
    SinglyLinkedNode* node = tasks.getHead();

    while(node != NULL)
    {
      // TODO: assert IScheduled* does indeed match
      ITask& task = (ITask&) *node;

      task.execute();

      node = node->getNext();
    }
  }

  void ScheduleManager::insertAfter(IScheduled& justBefore, IScheduled& scheduled)
  {
#if DEBUG
      void* _justBefore = &justBefore;
      uint16_t beforeCount = listScheduled.count();

      if (_justBefore != NULL)
      {
          SinglyLinkedNode* justAfter = justBefore.getNext();

          Serial << F("justBefore.getNext() is NULL = ") << (justAfter == NULL);
          Serial.println();
      }
      else
      {
          Serial << F("justBefore is NULL = true");
          Serial.println();
      }
#endif

    listScheduled.insertBetween(&justBefore, justBefore.getNext(), &scheduled);

#if DEBUG
    Serial << F("Now have: ") << listScheduled.count() << F(" scheduled, before had: ") << beforeCount;
    Serial.println();
#endif
  }

  void ScheduleManager::add(IScheduled& scheduled, IScheduled& justAfter)
  {
    // FIX: same as insertAfter ?
    //listScheduled.insertAfter(&justAfter, scheduled);
    //listScheduled.insertAfter(&justAfter, &scheduled);
  }

  void ScheduleManager::insert(IScheduled& justBefore, IScheduled& scheduled)
  {
    listScheduled.insert(&justBefore, &scheduled);
  }

  void ScheduleManager::execute()
  {
    IScheduled* scheduled = (IScheduled*) listScheduled.getHead();

#if DEBUG
    uint16_t loopCounter = 0;
#endif
    #if X_DEBUG
    Serial << F("Got scheduled: ");
    Serial.println();
    #endif

    //return;

    // service every scheduled whose time it is to wake up...
    while(scheduled != NULL && scheduled->isWakeupTime(timingGroup))
    {
      // hold on to 'next' reference since listScheduled.remove (kludgey style)
      // assigns next to NULL
      IScheduledAbsolute* scheduledNext = (IScheduledAbsolute*) scheduled->getNext();

#if DEBUG
      Serial << F("Woke up: ") << timingGroup.GetCurrentTime();
      Serial << F(" / loop counter = ") << ++loopCounter;
      Serial.println();
#endif
      // then do our stuff
      scheduled->execute();

      timingGroup.lastActionTimeFinish = timingGroup.GetCurrentTime();

      // TODO: optimize this.  Calling remove like this is slow
      // remove ourselves from the buffer
      listScheduled.remove(scheduled);

      scheduled->schedule(this);

#if DEBUG_VERBOSE
      printDebug();
#endif

      scheduled = scheduledNext;
    }
  }

#if DEBUG
  void ScheduleManager::printDebug()
  {
      IScheduled* scheduled = (IScheduled*)listScheduled.getHead();

      Serial << F("--- schedule manager --- \r\n");

      while (scheduled != NULL)
      {
          scheduled->printDebug();

          scheduled = (IScheduled*) scheduled->getNext();
      }
  }
#endif

  // find closest node who wakes up on or just after targetTime
  IScheduled* ScheduleManager::findNodeOnOrJustAfter(uint32_t targetTime)
  {
    SinglyLinkedNode* node = listScheduled.getHead();

    // search through all nodes, starting with head
    while(node != NULL)
    {
      IScheduled* scheduled = (IScheduled*) node;

      // compare wakeup time per node
      char compared = scheduled->compareWakeupTime(targetTime);
      // schedule wants to wake up now or after now?
      // > 0 if scheduled node's wakeup time is > targetTime
      if(compared >= 0)
        // yes, return our findings.  Returning imeediately gauruntees
        // we grab the earliest and therefore best candidate
        return scheduled;

      node = node->getNext();
    }

    return NULL;
  }


  IScheduled* ScheduleManager::findNodeJustBefore(uint32_t targetTime)
  {
    SinglyLinkedNode* node = listScheduled.getHead();
    IScheduled* previousScheduled = NULL;

    // this we want to go up until we just pass the "just before"
    // node, then back up one and return our findings

    // iterate thru every node
    while(node != NULL)
    {
      IScheduled* scheduled = (IScheduled*) node;

      // compare wakeup time per node
      char compared = scheduled->compareWakeupTime(targetTime);

      // does this schedule want to wake up now or after now?
      if(compared >= 0)
        // yes, so that means the PREVIOUS one comes just before it
        // this works because we do this right away, vs. looping all thru
        // then returning
        return previousScheduled;

      node = node->getNext();
      previousScheduled = scheduled;
    }

    return NULL;
  }

  void ScheduleManager::add(IScheduled& scheduled)
  {
    scheduled.schedule(this);
  }

  void ScheduleManager::append(IScheduled& scheduled)
  {
    listScheduled.add(&scheduled);
  }

  // FIX: findNodeOnOrJustAfter seems to be working right, so schedule
  // itself is off.  I'm thinking sm->append should be something else
  void IScheduled::schedule(ScheduleManager* sm, uint32_t targetTime)
  {
    IScheduledAbsolute* onOrJustAfter = (IScheduledAbsolute*)
      sm->findNodeOnOrJustAfter(targetTime);

    // if we find a node which wakes up on or just after our target time
    if(onOrJustAfter != NULL)
      // slide this one just in before it
      sm->insert(*onOrJustAfter, *this);
    else
      // if no node is found on or after our target time, then it's an
      // empty list so append to end
      sm->append(*this);
  }

  void IScheduledAbsolute::begin(TimingGroup& tg)
  {
    absoluteWakeupTime = tg.GetCurrentTime();
  }

  bool IScheduledAbsolute::isWakeupTime(TimingGroup& tg)
  {
    return tg.GetCurrentTime() >= absoluteWakeupTime;
  }

  char IScheduledAbsolute::compareWakeupTime(uint32_t targetTime)
  {
    if(targetTime < absoluteWakeupTime) return 1;
    else if(targetTime > absoluteWakeupTime) return -1;
    else return 0;
  }

#if DEBUG
  void IScheduledAbsolute::printDebug()
  {
    Serial << F("IScheduledAbsolute: wakeup time = ") << absoluteWakeupTime;
    Serial.println();
  }
#endif
}
