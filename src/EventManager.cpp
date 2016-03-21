#include "EventManager.h"
#include "Console.Stream.h"

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
bool HandleManager::remove(Handle* handles, Handle* currentNode, void* data)
{
  Handle* prevNode = NULL;
  
  do
  {
    if(currentNode->getData() == data)
    {
      // very first currentNode is the match, which means we 
      // don't have a prevNode yet - so caller must handle it
      if(prevNode == NULL) return false;
      
      // redirect previoous node's next to startNode's next
      prevNode->next = currentNode->getNext();
      return true;
    }
    prevNode = currentNode;
    
    // TODO: optimize this part
    if(currentNode->getNext() == nullHandle)
      currentNode = NULL;
    else
      currentNode = getHandle(handles, currentNode->getNext());
      
  } while(currentNode);
  
  // undefined state
#ifdef DEBUG
  cout.println(F("HandleManager::remove "
#ifdef MEMORY_OPT_CODE
  "invalid op"));
#else
  "warning: data to remove not found"));
#endif
#endif
  return true; 
  // false would indicate that first node needs attention, but actually it's a soft error
  // so we return true which instructs the client to take no further action (because it
  // thinks the operation succeeded)
}


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

void HandleBase::remove(HandleManager* manager, void* data)
{
  HandleManager::Handle* h = manager->getHandle(handle);
  bool removed = HandleManager::remove(manager->handles, h, data);
  if(!removed)
    // we get here when first handle is to be removed
    handle = h->getNext();
}
