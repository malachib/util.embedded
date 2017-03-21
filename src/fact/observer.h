#pragma once

#include "CircularBuffer.h"

namespace FactUtilEmbedded
{
struct IObservable
{

};

struct IObserver
{
  virtual void observe(IObservable* observable) = 0;
};

/*
class ObservableLinkedList : IObservable
{
  DoublyLinkedList<IObserver> observers;

protected:
  void notify();
};
*/


struct IContainer
{

};

// Represents a source (subject) observable array
class ObservableArray : public IObservable, IContainer
{
  IObserver** observers;
  uint8_t length;

protected:
  void notify() { notify(this); }
  void notify(IObservable* observable);

public:
  ObservableArray(IObserver** observers, byte length) :
    observers(observers),length(length)
  { }

  ObservableArray() {}

  void add(IObserver* observer);

  void setObservers(IObserver** observers, byte length = 0)
  {
    this->observers = observers;
    if(length > 0) this->length = length;
  }
};


// represents a relay observer
// as an observer, listens for one event, then broadcasts to a list of observers
class ObserverArray : public ObservableArray,
  public IObserver
{
public:
  void observe(IObservable* subject) override;
};



// TODO: Work out message queing
template <class T>
class ObserverMessenger : public IObserver
{
  layer3::CircularBuffer<T> messages;
protected:
  virtual T getMessage(IObservable* observable) = 0;
public:
  void observe(IObservable* observable) override;

  ObserverMessenger(T* messages, int size)
    : messages(messages, size)
    {}

  T get() { return messages.get(); }

  uint16_t available() { return messages.available(); }
};

template <class T>
void ObserverMessenger<T>::observe(IObservable* observable)
{
  T message = getMessage(observable);
  messages.put(message);
}
}
