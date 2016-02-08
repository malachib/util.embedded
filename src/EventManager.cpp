#include "EventManager.h"

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

HandleManager::handle HandleManager::add(HandleManager::handle h, void* data)
{
  Handle& _h = handles[h];

  // look for any free ones (somewhat CPU expensive, but we shouldn't be adding/removing)
  // events so often - mainly firing them
  for(uint8_t i = 0; i < 10; i++)
  {
    Handle& hEval = handles[i];

    if(hEval.getData() == NULL)
    {
      _h.next = i;
      hEval.data = data;
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
