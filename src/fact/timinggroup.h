#ifndef _mal_timinggroup
#define _mal_timinggroup

#include "lib.h"

namespace FactUtilEmbedded
{
  class TimingGroupBase
  {
public:
      virtual uint32_t GetCurrentTime() = 0;
  };

  class ScheduleManager;

  class TimingGroup //: TimingGroupBase
  {
    // FIX: hacky, needed for direct access to lastActionTimeFinish
    friend ScheduleManager;

    typedef uint32_t (*getCurrentTimeFunc)(void);
    static uint32_t GetCurrentTimeMillis()
    {
      return millis();
    }

    static uint32_t GetCurrentTimeMicro()
    {
      return micros();
    }

    public:
    // When we wanted to perform the last action
    uint32_t lastActionTimeTarget;
    // When we actually performed the last action
    uint32_t lastActionTimeFinish;

    //virtual long GetCurrentTime() { return millis(); }

    // unfortunately virtuals don't optimize well, so gonna FN-pointer this sucker
    //const long (*GetCurrentTime)(void); // this won't work because it only does a const FUNCTION and not a const FIELD (we want both)
    const getCurrentTimeFunc GetCurrentTime; // therefore we must do this

    TimingGroup(bool useMicro = false) : GetCurrentTime(
      useMicro ? GetCurrentTimeMicro : GetCurrentTimeMillis)
    {
    }

#ifdef DEBUG
    // For debug scenarios where we wish to have time not be realtime
    TimingGroup(getCurrentTimeFunc getCurrentTime) : GetCurrentTime(getCurrentTime)
    {
    }
#endif

    // Determine if we are at or after interval + lastActionTimeTarget
    bool AtOrAfterTargetTime(uint32_t interval)
    {
      return GetCurrentTime() > lastActionTimeTarget + interval;
    }

    // Determine if we are at or after interval + lastActionTimeTarget, and
    // if so optionally register the new action time
    bool IsIntervalReached(uint32_t interval, bool registerActionTime = true)
    {
      if(AtOrAfterTargetTime(interval))
      {
        lastActionTimeTarget += interval;
        return true;
      }
      return false;
    }

	void reset()
	{
		lastActionTimeTarget = GetCurrentTime();
	}

    uint32_t getLastActionTimeTarget()
    {
      return lastActionTimeTarget;
    }

    uint32_t getLastActionTimeFinish()
    {
      return lastActionTimeFinish;
    }
  };
}
#endif
