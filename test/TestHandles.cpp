#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include "EventManager.h"

using namespace FactUtilEmbedded;

class _HandleManager : public HandleManager
{
public:
  class Handle : public HandleManager::Handle
  {

  };
  
  bool isNextHandleNull(handle h)
  {
    auto _h = getHandle(h);
    return _h->getNext() == nullHandle;
  }
};

SCENARIO("Handle subsystem", "[handles]")
{
  GIVEN("A base-class handle manager")
  {
    _HandleManager handleManager;
    handleManager.init();
    HandleManager::handle handle1, handle2;

    char val1[] = "1";
    char val2[] = "2";

    //WHEN("Operating on handle1")
    {
      handle1 = handleManager.init(val1);

      REQUIRE(handle1 == 1);
    }
    //WHEN("Operating on handle2")
    {
      handle2 = handleManager.add(handle1, val2);

      REQUIRE(handle2 == 2);
    }
    WHEN("Operating on both handles together")
    {
      auto _handle1 = (_HandleManager::Handle*) handleManager.getHandle(handle1);

      REQUIRE(_handle1->getData() == val1);

      auto _handle2 = (_HandleManager::Handle*) handleManager.getHandle(_handle1->getNext());
      REQUIRE(_handle2->getData() == val2);
    }

    REQUIRE(handleManager.available() == HANDLEMANAGER_CAPACITY - 2);

    handleManager.clear(handle1); // this should wipe out handle2 also

    REQUIRE(handleManager.available() == HANDLEMANAGER_CAPACITY);

    {
      handle1 = handleManager.init(val1);

      REQUIRE(handle1 == 1);
    }
    //WHEN("Operating on handle2")
    {
      handle2 = handleManager.add(handle1, val2);

      REQUIRE(handle2 == 2);
    }
    
    WHEN("Removing one handle by data value")
    {
      bool result = handleManager.remove(handle1, val1);
      
      // can't remove first one because it doesn't know where to find
      // prev item, so should be false
      REQUIRE(result == false);

      result = handleManager.remove(handle1, val2);
      
      // this one should be removable
      REQUIRE(result == true);
      REQUIRE(handleManager.isNextHandleNull(handle1));
    }
    //handleManager.remove(handle1);

    //REQUIRE(handleManager.available() == HANDLEMANAGER_CAPACITY - 1);

  }
}
