#pragma once

#include <cstdint>

namespace FactUtilEmbedded
{
  namespace layer1
  {
    class DateModel
    {
      uint8_t day;
      uint8_t month;
      uint16_t year;
      
    public:
      uint8_t getDay() { return day; }
      uint8_t getMonth() { return month; }
      uint16_t getYear() { return year; }
    };
    
    template <class TDateModel>
    class Date
    {
      TDateModel dateModel;
    public:
      
      uint8_t getDay() { return dateModel.getDay(); }
      uint8_t getMonth() { return dateModel.getMonth(); }
      uint16_t getYear() { return dateModel.getYear(); }
    };
    
    
    class TimeModel
    {
      struct
      {
        uint16_t hours : 4;
        uint16_t minutes : 6;
        uint16_t seconds : 6;
      };
    };
    
    // returns number of days added by adding these seconds to time
    template <class TTimeModel, class TUnit>
    uint16_t tm_addseconds(TTimeModel* model, TUnit seconds)
    {
      uint16_t addDays = 0;
      uint8_t addHours = 0;
      uint8_t addMinutes = 0;
      
      // if we know we'll overflow by at least 24 hours...
      if(seconds > (3600 * 24))
      {
        addDays = seconds / (3600 * 24);
        seconds -= (addDays * (3600 * 24));
      }

      // if we know we'll overflow by at least an hour...
      if(seconds > 3600)
      {
        addHours = seconds / 3600;
        if(model->getHours() + addHours > 24)
        {
          
        }
        seconds -= ((uint16_t)addHours * 3600);
      }
      
      // if we know we'll overflow by at least a minute...
      if(seconds > 60)
      {
        addMinutes = seconds / 60;
        seconds -= ((uint16_t)addMinutes * 60);
      }
    }
    
    
    template <class TTimeModel>
    class Time
    {
      TTimeModel timeModel;
    public:
    };
    
    // .NET style , represents 100-nanosecond ticks since 
    // 12 midnight Jan 1 0001
    class TicksDateTimeModel
    {
      uint64_t ticks;
    };
    
    // Seconds elapsed since midnight, January 1 1970
    class UnixDateTimeModel
    {
      time_t time;
      
    public:
      //uint8_t getDay() { return 0; }
    };
    

    template <class TDateTimeModel>
    class DateTime
    {
      TDateTimeModel dateTimeModel;
    public:
      
      uint8_t getDay() { return dateTimeModel.getDay(); }
    };
  }
}
