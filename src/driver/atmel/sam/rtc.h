#pragma once

#include <Arduino.h>

#if SAMD_SERIES == 0
#error This code intended only for ATSAMD MPU
#else

namespace FactUtilEmbedded
{
  namespace atmel
  {
  namespace sam
  {
    // lifted and adapted from https://github.com/arduino-libraries/RTCZero
    class RealTimeClock
    {
      inline static bool is_mode2_synching()
      {
        return RTC->MODE2.STATUS.bit.SYNCBUSY;
      }
      
      inline static void synchronize()
      {
        while(is_mode2_synching());
      }
      
      static void read_synchronize()
      {
        RTC->MODE2.READREQ.reg = RTC_READREQ_RREQ;
        synchronize();
      }
      
    public:
      static void begin();
      
      inline static uint8_t getSeconds(bool sync = true)
      {
        if(sync) read_synchronize();
        return RTC->MODE2.CLOCK.bit.SECOND;
      }

      inline static uint8_t getMinutes(bool sync = true)
      {
        if(sync) read_synchronize();
        return RTC->MODE2.CLOCK.bit.MINUTE;
      }

      inline static uint8_t getHours(bool sync = true)
      {
        if(sync) read_synchronize();
        return RTC->MODE2.CLOCK.bit.HOUR;
      }

      inline static uint8_t getDay(bool sync = true)
      {
        if(sync) read_synchronize();
        return RTC->MODE2.CLOCK.bit.DAY;
      }

      inline static uint8_t getMonth(bool sync = true)
      {
        if(sync) read_synchronize();
        return RTC->MODE2.CLOCK.bit.MONTH;
      }

      static uint8_t getYear(bool sync = true)
      {
        if(sync) read_synchronize();
        return RTC->MODE2.CLOCK.bit.YEAR;
      }
      
      inline static void setSeconds(uint8_t seconds, bool sync = true)
      {
        RTC->MODE2.CLOCK.bit.SECOND = seconds;
        if(sync) synchronize();
      }

      inline static void setMinutes(uint8_t minutes, bool sync = true)
      {
        RTC->MODE2.CLOCK.bit.MINUTE = minutes;
        if(sync) synchronize();
      }

      inline static void setHours(uint8_t hours, bool sync = true)
      {
        RTC->MODE2.CLOCK.bit.HOUR = hours;
        if(sync) synchronize();
      }

      static uint8_t getAlarmSeconds()
      {
        return RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND;
      }

      static uint8_t getAlarmMinutes()
      {
        return RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE;
      }

      static uint8_t getAlarmHours()
      {
        return RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR;
      }

      static uint8_t getAlarmDay()
      {
        return RTC->MODE2.Mode2Alarm[0].ALARM.bit.DAY;
      }

      static uint8_t getAlarmMonth()
      {
        return RTC->MODE2.Mode2Alarm[0].ALARM.bit.MONTH;
      }

      static uint8_t getAlarmYear()
      {
        return RTC->MODE2.Mode2Alarm[0].ALARM.bit.YEAR;
      }
      
      inline static void setAlarmSeconds(uint8_t seconds, bool sync = true)
      {
        RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND = seconds;
        if(sync) synchronize();
      }

      inline static void setAlarmMinutes(uint8_t minutes, bool sync = true)
      {
        RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE = minutes;
        if(sync) synchronize();
      }

      inline static void setAlarmHours(uint8_t hours, bool sync = true)
      {
        RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR = hours;
        if(sync) synchronize();
      }
      
      inline static void enable(bool sync = true)
      {
        RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE;
        if(sync) synchronize();
      }

      inline static void disable(bool sync = true)
      {
        RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_ENABLE;
        if(sync) synchronize();
      }
    };
  }
  }
}
#endif
