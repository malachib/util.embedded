#pragma once

namespace FactUtilEmbedded { namespace experimental {

typedef void (*responder_t)(int, int);

template <class T>
class Dependent
{
public:
    template <class ...TArgs>
    class On
    {
    public:
        typedef T t_t;

        template <responder_t responder, class TDM, int parent_id>
        static void _walk_across()
        {

        }

        template <responder_t responder, class TDM, int parent_id, class T2, class ...TArgs2>
        static void _walk_across()
        {
            _walk_across<responder, TDM, parent_id, TArgs2...>();

            responder(parent_id, T2::ID);
            // Now, here we need to dig into the On:: for each child represented by T2,
            // so that we can recursively walk down in and walk across from there (if needed)
            // so perhaps we need to retrieve that from the manager via the ID
            //TDM::get<parent_id>();
            TDM::template walk2<T2::ID, responder>();
        }

        // walks across children only, not parent
        template <responder_t responder, class TDM, int parent_id>
        static void walk_across()
        {
            _walk_across<responder, TDM, parent_id, TArgs...>();
        }
    };

};


template <class ...TArgs>
class DependencyManager
{
    /*
    template <void responder(int id), class Dependency>
    static void do_walk()
    {

    } */

    template <responder_t responder>
    static void do_walk()
    {

    }

    // We want T to be 'Dependent::On<...>' in this case
    template <responder_t responder, class T>
    static void handle_it()
    {
        typedef typename T::t_t t_t;
        constexpr int ID = t_t::ID;

        // Done from within 'On' class
        //responder(ID, 0);

        // As per https://stackoverflow.com/questions/2105901/how-to-fix-expected-primary-expression-before-error-in-c-template-code
        T::template walk_across<responder, DependencyManager<TArgs...>, ID>();
        //T::template _test<int>(5);
        //T::_test();
    }

    template <responder_t responder, class T, class ...TArgs2>
    static void do_walk()
    {
        do_walk<responder, TArgs2...>();

        handle_it<responder, T>();
    }

    template <int id>
    static void _get()
    {

    };

    template <int id, class T, class ...TArgs2>
    static void _get()
    {
        _get<id, TArgs2...>();
    }

    // walk over a specific ID only
    template <int id, responder_t responder>
    static void _walk2()
    {

    }

    // walk over a specific ID only
    template <int id, responder_t responder, class T, class ...TArgs2>
    static void _walk2()
    {
        typedef typename T::t_t t_t;
        constexpr int ID = t_t::ID;

        if(ID == id)
        {
            //responder(0, id);
            T::template walk_across<responder, DependencyManager<TArgs...>, ID>();
        }

        _walk2<id, responder, TArgs2...>();
    }

public:

    template <responder_t responder>
    static void walk()
    {
        do_walk<responder, TArgs...>();
    }

    // Get the 'On' class for this particular ID
    template <int id>
    void get() {}
    /*
    T get() -> decltype()
    {
        return 5;
    }
     */

    // walk over everything under a specific ID
    template <int id, responder_t responder>
    static void walk2()
    {
        _walk2<id, responder, TArgs...>();
    }
};

}}