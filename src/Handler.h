#pragma once

#include <LinkedList.h>

namespace FactUtilEmbedded
{
  struct IHandler
  {
  };

  // TODO: fix naming
  template <class TIn, class TOut>
  struct IHandler2
  {
    virtual TOut canHandle(TIn input) = 0;
    //virtual bool execute(TIn input) = 0;
  };


  template <class TIn, class TOut>
  class IHandlerAggregator : IHandler2<TIn, TOut>
  {
    SinglyLinkedList handlers;

    // TODO: use this if it doesn't macro-expand over and over again
    // I'm hoping it only expands once per TIn,TOut combo
  public:
    virtual IHandler2<TIn, TOut> canHandle(TIn input) override
    {
      SinglyLinkedNode* menu = handlers.getHead();

      for(;menu != NULL; menu = menu->getNext())
      {
        if(((IHandler2<TIn, TOut>)menu)->canHandle(input))
          return (IHandler2<TIn, TOut>) menu;
      }

      return NULL;
    }
  };
}
