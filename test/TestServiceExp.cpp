#include "catch.hpp"

#include <experimental/Dependency.h>
#include <experimental/Service.h>

using namespace FactUtilEmbedded::experimental;

class MostImportantService : public Service
{
public:
    void start()
    {
    }


    void stop()
    {

    }


    void loop()
    {

    }
};

SCENARIO( "Experimental service code", "[exp-service]" )
{
    GIVEN("Test #1")
    {
    }
}