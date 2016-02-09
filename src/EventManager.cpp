#include "EventManager.h"

EventManager eventManager;

/*
HandleManager::handle HandleManager::findFree()
{
  for(uint8_t i = 0; i < 10; i++)
  {
    Handle& hEval = handles[i];

    if(hEval.getData() == NULL)
    {
      return i;
    }
  }

  return nullHandle;
}*/

// initialize a new list of handles starting with the first item
HandleManager::handle HandleManager::init(void* data)
{
  // look for any free ones (somewhat CPU expensive, but we shouldn't be adding/removing)
  // events so often - mainly firing them
  for(uint8_t i = 0; i < HANDLEMANAGER_CAPACITY; i++)
  {
    Handle& hEval = handles[i];

    if(hEval.getData() == NULL)
    {
      hEval.data = data;
      hEval.next = nullHandle;
      return i + 1;
    }
  }
}

// add a handle onto an already initialized list of handles
HandleManager::handle HandleManager::add(HandleManager::handle h, void* data)
{
  Handle* lastHandle;

  // finds last handle in chain - CPU expensive
  while(h != nullHandle)
  {
    lastHandle = getHandle(h);
    h = lastHandle->getNext();
  }

  // look for any free ones (somewhat CPU expensive, but we shouldn't be adding/removing)
  // events so often - mainly firing them
  for(uint8_t i = 0; i < HANDLEMANAGER_CAPACITY; i++)
  {
    Handle& hEval = handles[i];

    if(hEval.getData() == NULL)
    {
      lastHandle->next = i + 1; // remember we bump forward one so that we can use 0 as NULL handle
      hEval.data = data;
      hEval.next = nullHandle;
      return i + 1;
    }
  }
}

void EventManager::invoke(HandleManager::handle h, void* parameter)
{
  while(h != nullHandle)
  {
    Event* event = getEvent(h);
    event->getCallback()(parameter);
    h = event->getNext();
  }
}

void HandleBase::add(HandleManager* manager, void* data)
{
  if(handle == HandleManager::nullHandle)
    handle = manager->init(data);
  else
    manager->add(handle, data);
}
