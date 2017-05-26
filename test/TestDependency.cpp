#include "catch.hpp"

#include <experimental/Dependency.h>
#include <experimental/Service.h>

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
        Dependent<A>::On<>,
        Dependent<BdependsOnA>::On<A>,
        Dependent<CdependsOnB>::On<BdependsOnA>,
        Dependent<DdependsOnBandC>::On<BdependsOnA, CdependsOnB>> dm;

DependencyManager<
        Dependent<BdependsOnA>::On<A>> dm2;

void responder(int parent_id, int id)
{
    printf("Inspecting: Parent = %d, id = %d\r\n", parent_id, id);
}

class TestContext
{
public:
    template <class TParent, class T>
    static bool top_callback()
    {
        return true;
    }

    template <class TParent, class T>
    static void callback()
    {
        constexpr auto id = T::ID;
        printf("Inspecting: id = %d\r\n", id);
    }
};

SCENARIO( "Experimental dependency code", "[exp-dependency]" )
{
    GIVEN("Test #1")
    {
        //dm.walk<responder>();
        //dm.walk2<4, responder>();
        dm.walk3<4, responder>();

#ifdef UNUSED
        auto val_on = dm.get<4>();
        typedef decltype(val_on) val_on_t;
        auto id = val_on_t::t_t::ID;
        // annoyingly always returns 2.  Presumably because decltype doesn't actually
        // run the method
        printf("Test id = %d", id);
#endif

        //dm.walk4<4, responder>();
        dm.walk5<4, TestContext, 4>();

    }
    GIVEN("Template compile-time vector tests")
    {
        int index = 0;
        //const int index2 = index;

        typedef VectorBase<long, 100, 101, 102> vector_t;

        auto value = vector_t::get(0);
        value = vector_t::get(1);
        value = vector_t::get(2);
        value = vector_t::get(3);
        value = vector_t::get(index);
        value = vector_t::get(4);

        value = vector_t::get<0>();
        value = vector_t::get<1>();
        value = vector_t::get<2>();
        //value = vector_t::get<index2>();
        //value = vector_t::get<4>();
    }
}