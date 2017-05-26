#include "catch.hpp"

#include <experimental/Dependency.h>

using namespace FactUtilEmbedded::experimental;

class A
{
public:
    static constexpr int ID = 1;
};


class BdependsOnA
{
public:
    static constexpr int ID = 2;
};


class CdependsOnB
{
public:
    static constexpr int ID = 3;
};


class DdependsOnBandC
{
public:
    static constexpr int ID = 4;
};


// these 3 instances are not necessary at this time but keeping since
// things are experimental
Dependent<BdependsOnA>::On<A> val0;
Dependent<CdependsOnB>::On<BdependsOnA> val1;
Dependent<DdependsOnBandC>::On<BdependsOnA, CdependsOnB> val2;


DependencyManager<
        Dependent<BdependsOnA>::On<A>,
        Dependent<CdependsOnB>::On<BdependsOnA>,
        Dependent<DdependsOnBandC>::On<BdependsOnA, CdependsOnB>> dm;

DependencyManager<
        Dependent<BdependsOnA>::On<A>> dm2;

void responder(int parent_id, int id)
{
    printf("Inspecting: Parent = %d, id = %d\r\n", parent_id, id);
}

SCENARIO( "Experimental dependency code", "[exp-dependency]" )
{
    GIVEN("Test #1")
    {
        //dm.walk<responder>();
        //dm.walk2<4, responder>();
        dm.walk3<4, responder>();
        auto val_on = dm.get<4>();
        typedef decltype(val_on) val_on_t;
        auto id = val_on_t::t_t::ID;
        // annoyingly always returns 2.  Presumably because decltype doesn't actually
        // run the method
        printf("Test id = %d", id);
    }
}