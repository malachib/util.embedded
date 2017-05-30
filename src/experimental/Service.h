#pragma once

//#include <tuple>

#include "Dependency.h"
#include "../Service.h"
#include "Tuple.h"

namespace fstd = ::std;

namespace FactUtilEmbedded { namespace experimental {

class Service
{
public:
    template <class ...TArgs>
    class DependsOn : public Vector2<TArgs...>
    {

    };

    enum State
    {
        Stopped,
        Initializing,
        Running,
        Paused,
        Sleeping,
        Shutdown, // in process of shutting down
        Error
    };

    // TODO: substate may not be best place to track the transition
    // of one major state to another.  By definition it's temporary
    // so perhaps we can have a context instance floating around to
    // hold onto it and a set of specialized template functions to
    // ensure validity of one move to the next

    enum class StoppedState
    {
        Unstarted,
        Starting,
        Finished  // reach here after a shutdown only
    };

    enum class SleepingState
    {
        Sleeping,
        Waking
    };

    enum class RunningState
    {
        Running,
        Pausing, // entering pause state
        EnteringSleep,
        Shutdown, // entering shutdown state
    };


    enum class PausedState
    {
        Paused,
        Unpausing
    };


    enum class InitializingState
    {
        Initializing,
        Retrying, // held up in exceptionally long retry conditions
        Starting // Initialized and moving to running state
    };


    enum class ShutdownState
    {
        ShuttingDown,
        Retrying, // held up in exceptionally long retry conditions
        Stopping // All shut down and moving to stopped state
    };


    union SubState
    {
        StoppedState stoppedState;
        PausedState pausedState;
        RunningState runningState;
        SleepingState sleepingState;
        uint8_t errorState;
    };

    struct
    {
        uint8_t _state : 3;
        uint8_t sub_state : 3;
        uint8_t status_custom : 1;
        uint8_t attn_subsystem : 1;
    };

protected:
    void state(State s)
    {
        _state = s;
    }

    void state(StoppedState s) { sub_state = static_cast<int>(s); }
    void state(PausedState s) { sub_state = static_cast<int>(s); }
    void state(RunningState s) { sub_state = static_cast<int>(s); }
    void state(SleepingState s) { sub_state = static_cast<int>(s); }
    void state(InitializingState s) { sub_state = static_cast<int>(s); }

public:

    template <class TSubState> TSubState state();

    uint8_t error_code() { return sub_state; }
    void error_code(uint8_t code) { sub_state = code; }

    State state() { return static_cast<State>(_state); }

    template <class TSubState> void state(const State&& s, const TSubState&& sub_state)
    {
        _state = s;
        this->sub_state = sub_state;
    }

    bool subsystem_attention() { return attn_subsystem; }
    void subsystem_attention(bool value) { attn_subsystem = value; }
};

template<>
inline Service::StoppedState Service::state<Service::StoppedState>()
{ return static_cast<StoppedState>(sub_state); }

template<>
inline Service::PausedState Service::state<Service::PausedState>() { return static_cast<PausedState>(sub_state); }

template<>
inline Service::RunningState Service::state<Service::RunningState>() { return static_cast<RunningState>(sub_state); }

template<>
inline Service::SleepingState Service::state<Service::SleepingState>() { return static_cast<SleepingState>(sub_state); }

template<>
inline Service::InitializingState Service::state<Service::InitializingState >() { return static_cast<InitializingState >(sub_state); }

/*
class Service : public ServiceStateSplitBase<ServiceBase::State, ServiceBase::SubState>
{

}; */


template <class TService>
class ServiceContainerBase
{
public:
    constexpr static int ID = TService::ID;

    // FIX: temporary, this means we can only have one of each type of service for now
    // once we get DependencyManager working with instances, then we can switch this around also
    static TService service;
};

template <class TService>
class ServiceContainer : public ServiceContainerBase<TService>
{
public:
    static constexpr bool is_blocking = true;
};


template <class TService>
class ServiceContainerNonBlocking : public ServiceContainerBase<TService>
{
public:
    static constexpr bool is_blocking = false;

    //static TService& get_service() { return service; }
};



template <class ...TArgs>
class ServiceManager : public DependencyManager<TArgs...>
{
    typedef DependencyManager<TArgs...> base_t;

    static void check_status(int parent_id, int id)
    {

    }

    struct CheckStatusContext
    {
        template <class TParent, class TServiceContainer>
        static bool top_callback()
        {
            constexpr int parent_id = TParent::ID;
            constexpr int id = TServiceContainer::ID;

            typedef Service::State state_t;

            state_t state = TServiceContainer::service.state();

            fstd::clog << "Checking status on service " << id << fstd::endl;

            switch(state)
            {
                case state_t::Running:
                    // If state is running OK, then we assume subsystems are running OK too so
                    // return false to abort any further digging - unless subsystems need attention
                    return TServiceContainer::service.subsystem_attention();

                case state_t::Error:
                    if(parent_id != id)
                        TParent::service.subsystem_attention(true);
                    break;

                default:
                    // Sleeping, Paused we ignore and also don't check subsystem status
                    return false;
            }

            return true;
        }

        template <class TParent, class TServiceContainer>
        static void callback()
        {
            constexpr int parent_id = TParent::ID;
            constexpr int id = TServiceContainer::ID;

            fstd::clog << "Entering service " << id << "(parent: " << parent_id << ")" << fstd::endl;
        }
    };


    struct LoopContext
    {
        template <class TParent, class TServiceContainer>
        static bool top_callback()
        {
            return true;
        }

        template <class TParent, class TServiceContainer>
        static void callback()
        {
            TServiceContainer::service.loop();
        }
    };

public:
    template <int id>
    static void loop()
    {
        base_t::template walk5<id, CheckStatusContext, id>();
        base_t::template walk5<id, LoopContext, id>();
        //walk3<id, check_status>();
    }


    template <class TService>
    static void loop()
    {
        loop<TService::ID>();
    }
};


template <class TValue>
class Singleton
{
public:
    static TValue service;

};


template <class ...TArgs>
class SingletonVector
{
public:
    template <class T>
    T get()
    {

    }

};

template <class TService, class ...TServices>
class ServiceManager2;

template <class TService, class ...TServices>
TService& get_service(ServiceManager2<TService, TServices...>& sm);

// TODO: if we can, make this all use tuples instead
// probably it's possible, I just don't know enough yet
// (I know how to declare a tuple, but iterating thru the stuff the way I want is a bit beyond me)

template <class TService, class ...TServices>
class ServiceManager2 : public ServiceManager2<TServices...>
{
    typedef ServiceManager2<TServices...> base_t;
public:
    TService service;
    //std::tuple<TServices...> test;

    template <typename CompletionHandler>
    void dispatch(CompletionHandler handler)
    {
        handler(service);

        base_t::dispatch(handler);
    }

    template <class ...TServicePtrs>
    void get_service(TService*& output, TServicePtrs...ptrs)
    {
        output = &service;
        base_t::get_service(ptrs...);
    }

    /*
    template <typename CompletionHandler>
    //template <typename CompletionHandler>
    void dispatch2(CompletionHandler handler)
    {
        //TServicePtrs...service_ptrs;
        //TService* svc1;
        //TServices... svcs;

        //get_service(svc1);

        //handler(service);
    } */

    template <typename TInstance, typename CompletionHandler, class ...TServicePtrs>
    void dispatch2_inst(TInstance* instance, CompletionHandler handler, TServicePtrs...service_ptrs)
    {
        base_t::dispatch2_inst(instance, handler, service_ptrs..., &service);
    }

    template <typename CompletionHandler, class ...TServicePtrs>
    void dispatch2(CompletionHandler handler, TServicePtrs...service_ptrs)
    {
        base_t::dispatch2(handler, service_ptrs..., &service);
    }

    template <typename CompletionHandler, class TInstance>
    void _dispatch2_inst(CompletionHandler handler, TInstance* instance)
    {
        dispatch2_inst(instance, handler);
    }

    template <typename CompletionHandler>
    void _dispatch2(CompletionHandler handler)
    {
        dispatch2(handler);
    }


    template <typename CompletionHandler, class TServicePtr, class ...TServicePtrs>
    void dispatch3(CompletionHandler handler)
    {
        TService* service_ptr = &service;

        dispatch2(handler, service_ptr, &base_t::service);
    }

    template <typename CompletionHandler>
    void dispatch3(CompletionHandler handler)
    {
        TService* service_ptr = &service;

        dispatch3<TService, TServices...>(handler);
    }


    void dispatch4()
    {
        std::experimental::Tuple<const TService*, const TServices*...> services;

        //std::experimental::get<0>(services) = nullptr;
    }

};


template <class TService>
class ServiceManager2<TService>
{
public:
    TService service;
    //std::tuple<TServices...> test;

    template <typename CompletionHandler>
    void dispatch(CompletionHandler handler)
    {
        handler(service);
    }

    template <typename CompletionHandler>
    void dispatch2(CompletionHandler handler)
    {
        handler(service);
    }


    template<typename T>
    struct tovoidptr { typedef void* type; };

    template <typename CompletionHandler, class ...TServicePtrs>
    void __dispatch2(CompletionHandler handler, TServicePtrs...service_ptrs)
    {
        handler(service_ptrs...);
    }

    template <typename TInstance, typename CompletionHandler, class ...TServicePtrs>
    void __dispatch2_inst(TInstance* instance, CompletionHandler handler, TServicePtrs...service_ptrs)
    {
        //typedef decltype(&TInstance) i_t;
        //typedef void (TInstance::callback_t)(TServicePtrs...);

        //callback_t c = handler;

        ((*instance).*handler)(service_ptrs...);
    }


    template <typename TInstance, typename CompletionHandler, class ...TServicePtrs>
    void dispatch2_inst(TInstance* instance, CompletionHandler handler, TServicePtrs...service_ptrs)
    {
        __dispatch2_inst(instance, handler, service_ptrs..., &service);
    }

    template <typename CompletionHandler, class ...TServicePtrs>
    void dispatch2(CompletionHandler handler, TServicePtrs...service_ptrs)
    {
        __dispatch2(handler, service_ptrs..., &service);
    }

    void get_service(TService*& output)
    {
        output = &service;
    }
};


// Copying from boost
class io_service
{
public:
    fstd::size_t run_one();
};


template <class TService, class ...TServices>
TService& get_service(ServiceManager2<TService, TServices...>& sm)
{
    return sm.service;
}


}}
