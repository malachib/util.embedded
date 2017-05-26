#include "catch.hpp"

#include <experimental/Dependency.h>
#include <experimental/Service.h>

using namespace FactUtilEmbedded::experimental;

class MostImportantService : public Service
{
public:
    static constexpr int ID = 1;

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

class FakeWifiService : public Service
{
public:
    typedef DependsOn<MostImportantService> depends_on_t;

    template <class TService>
    void on_dependent_state_change();

    static constexpr int ID = 3;

    void start()
    {

    }


    void loop()
    {

    }
};

class FakeRadioService : public Service
{
public:
    static constexpr int ID = 4;

    void start()
    {

    }


    void loop()
    {

    }
};

class AppService : public Service
{
public:
    static constexpr int ID = 5;

    void start()
    {

    }


    void loop()
    {

    }
};


namespace FactUtilEmbedded { namespace experimental {

template<class T>
T ServiceContainerBase<T>::service;

//template<>
//MostImportantService ServiceContainerBase<MostImportantService>::service;

//ServiceContainer<MostImportantService> fixme;

}}

ServiceManager<
        Dependent<ServiceContainer<MostImportantService>>::On<>,
        Dependent<ServiceContainer<FakeRadioService>>::On<ServiceContainer<MostImportantService>>,
        Dependent<ServiceContainer<FakeWifiService>>::On<
                ServiceContainer<MostImportantService>, ServiceContainer<FakeRadioService>>,
        Dependent<ServiceContainer<AppService>>::On<
                ServiceContainer<MostImportantService>, ServiceContainer<FakeWifiService>>
        > sm;

SCENARIO( "Experimental service code", "[exp-service]" )
{
    GIVEN("Test #1")
    {
        sm.loop<AppService>();
    }
}