#pragma once

#include "Dependency.h"
#include "../Service.h"

namespace FactUtilEmbedded { namespace experimental {

class Service
{
public:
    enum State
    {
        Stopped,
        Running,
        Paused,
        Sleeping,
        Error
    };

    enum class StoppedState
    {
        Unstarted,
        Starting,
        Finished
    };

    enum class SleepingState
    {
        Sleeping,
        Waking
    };

    enum class RunningState
    {
        Running,
        Pausing,
        EnteringSleep,
        Stopping
    };


    enum class PausedState
    {
        Paused,
        Unpausing
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
        uint8_t _state : 4;
        uint8_t sub_state : 4;
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
        template <class TServiceContainer>
        static void callback()
        {
            typedef Service::State state_t;

            state_t state = TServiceContainer::service.state();

            constexpr int id = TServiceContainer::ID;

            std::clog << "Checking status on service " << id << std::endl;

            switch(state)
            {
                case state_t::Running:
                    return;

                default:
                    // Attention required
                    return;
            }
        }
    };

public:
    template <int id>
    static void loop()
    {
        base_t::template walk5<id, CheckStatusContext, id>();
        //walk3<id, check_status>();
    }


    template <class TService>
    static void loop()
    {
        loop<TService::ID>();
    }
};

}}