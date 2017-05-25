#include "catch.hpp"

#include <experimental/Dependency.h>

using namespace FactUtilEmbedded::experimental;

class A
{

};


class BdependsOnA
{

};


class CdependsOnB
{

};


class DdependsOnBandC
{

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

SCENARIO( "Experimental dependency code", "[exp-dependency]" )
{

}