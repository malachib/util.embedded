#pragma once

namespace FactUtilEmbedded
{
  const uint16_t profileCount = 50;

  class Profiler
  {
    uint16_t iterator; // how many times Profile() was called before we get iterator results, which then clears it
    bool overflowed;
    uint32_t lastTimestamp;
    uint16_t intervals[profileCount];
    uint8_t frequency; // how often to actually record the interval, so that we can stretch profiling further over time without upping profileCount
    uint8_t frequencyCounter; // how far in we are to frequency
    //CircularBuffer<uint32_t> circularBuffer((uint32_t*)NULL, 10);
    layer3::CircularBuffer<uint16_t> buffer;
    void (*spikeObserver)(Profiler&);
    bool spikeObserverLatch = false;

    uint16_t max, min;

    uint16_t _maxs[5];
    layer3::CircularBuffer<uint16_t> maxs;

  public:
    Profiler() :
      buffer(intervals, profileCount),
      maxs(_maxs, 5)
    {
      frequencyCounter = 0;
      frequency = 200;
      ResetMinMax();
    }
    void Profile();

    void ResetMinMax()
    {
      max = 0;
      min = -1;
      maxs.reset();
    }

    void setSpikeObserver(void (*spikeObserver)(Profiler&))
    {
      this->spikeObserver = spikeObserver;
    }

    uint16_t getAverage();

    uint32_t getIterator()
    {
      uint32_t value = overflowed ? 0xFFFF : iterator;
      overflowed = false;
      iterator = 0;
      return value;
    }

    layer3::CircularBuffer<uint16_t>& getMaxs() { return maxs; }

    uint16_t getMin() { return min; }
    uint16_t getMax() { return max; }

    void debugPrint() { buffer.debugPrint(); }
  };
}
