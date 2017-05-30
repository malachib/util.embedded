#include "catch.hpp"

#include <experimental/Dependency.h>
#include <experimental/Service.h>
#include <experimental/Tuple.h>

using namespace FactUtilEmbedded::experimental;

using namespace FactUtilEmbedded::std::experimental;

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

/*template <class TService>
void dispatch_handler(TService service)
{

} */


void dispatch_handler(MostImportantService* svc1, FakeWifiService* svc2)
{

}

struct FakeService
{
    void start(MostImportantService* svc1, FakeWifiService* svc2)
    {

    }
};

SCENARIO( "Experimental service code", "[exp-service]" )
{
    GIVEN("Test #1")
    {
        sm.loop<AppService>();
    }
    GIVEN("Misplaced tuple test")
    {
        Tuple<int, float> val(1, 2.0);

        REQUIRE(get<0>(val) == 1);
        REQUIRE(get<1>(val) == 2.0);
        //REQUIRE(get<2>(val) == 0);
    }
    GIVEN("ServiceManager2")
    {
        FakeWifiService* svc1;
        MostImportantService* svc2;

        ServiceManager2<MostImportantService, FakeWifiService> sm2;

        auto id1 = get_service<MostImportantService>(sm2).ID;
        auto id2 = get_service<FakeWifiService>(sm2).ID;
        //auto id3 = get_service<FakeRadioService>(sm2).ID;

        FakeService fakeService;

        sm2.get_service(svc2, svc1);
        sm2._dispatch2(dispatch_handler);
        sm2._dispatch2_inst(&FakeService::start, &fakeService);
        //sm2._dispatch2([&]() -> { fakeService.start(); })
        sm2.dispatch4();

        REQUIRE(id1 == 1);
        REQUIRE(id2 == 3);

        /* needs C++14
        auto _dispatch = [](auto service) {

        }; */

        //sm2.dispatch(_dispatch);
        //sm2.dispatch(dispatch_handler);

        //REQUIRE(id3 == 4);
        //== 1);
        //REQUIRE(get_service<FakeWifiService>(sm2).ID == 3);
    }
}
