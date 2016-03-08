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

void HandleManager::init()
{
  // look for any free ones.  somewhat CPU expensive, but we shouldn't be adding/removing
  // events so often - mainly firing them
  for(uint8_t i = 0; i < HANDLEMANAGER_CAPACITY; i++)
  {
    Handle& hEval = handles[i];

    hEval.data = NULL;
  }
}


uint8_t HandleManager::available()
{
  uint8_t avail = 0;

  // look for any free ones.  somewhat CPU expensive, but we shouldn't be adding/removing
  // events so often - mainly firing them
  for(uint8_t i = 0; i < HANDLEMANAGER_CAPACITY; i++)
  {
    Handle& hEval = handles[i];

    if(hEval.data == NULL)
      avail++;
  }

  return avail;
}



// initialize a new list of handles starting with the first item
HandleManager::handle HandleManager::init(void* data)
{
  return alloc(data);
}

// add a handle onto an already initialized list of handles
HandleManager::handle HandleManager::add(HandleManager::handle h, void* data)
{
  Handle* lastHandle;

#ifdef FACT_LIB_STRICT
  if(h == nullHandle) return nullHandle;
#endif

  // finds last handle in chain - CPU expensive
  while(h != nullHandle)
  {
    lastHandle = getHandle(h);
    h = lastHandle->getNext();
  }

  return lastHandle->next = alloc(data);
}


// need to look thru entire list to build list backwards, so that
// we can find head node -- OR, pass in head node to begin with
/*
void HandleManager::remove(handle h)
{
  Handle* hToRemove = getHandle(h);

}
*/


void HandleManager::clear(handle h)
{
  while(h != nullHandle)
  {
    Handle* hToRemove = getHandle(h);
    h = hToRemove->getNext();
    hToRemove->data = NULL;
    hToRemove->next = nullHandle;
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
