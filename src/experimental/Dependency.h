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

        template <responder_t responder, int parent_id>
        static void handle_it()
        {

        }

        template <responder_t responder, int parent_id, class T2, class ...TArgs2>
        static void handle_it()
        {
            handle_it<responder, parent_id, TArgs2...>();

            responder(parent_id, T2::ID);
        }

        template <responder_t responder, int parent_id>
        static void _handle_it()
        {
            handle_it<responder, parent_id, TArgs...>();
        }


        static void _test()
        {
            _handle_it<nullptr, -1>();
        }

        template <class T2>
        static void _test(T2 value)
        {
            _handle_it<nullptr, -1>();
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
        T::template _handle_it<responder, ID>();
        //T::template _test<int>(5);
        //T::_test();
    }

    template <responder_t responder, class T, class ...TArgs2>
    static void do_walk()
    {
        do_walk<responder, TArgs2...>();

        handle_it<responder, T>();
    }

public:

    template <responder_t responder>
    static void walk()
    {
        do_walk<responder, TArgs...>();
    }
};

}}