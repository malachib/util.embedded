#pragma once

#include "LinkedList.h"

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

  struct IToggleHandler : IHandler
  {
    virtual void On() = 0;
    virtual void Off() = 0;
  };

  struct ICyclingHandler : IHandler
  {
    // think in terms of state machines:
    // return true if full operation has completed
    // return false if operation still in process
    virtual bool ProcessCycle() = 0;
  };

  struct IVectorHandler : IHandler
  {
    // Gets fed a value which moves around based on an external algorithm
    virtual bool ProcessVector(int vector) = 0;
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

      for(;menu != nullptr; menu = menu->getNext())
      {
        if(((IHandler2<TIn, TOut>)menu)->canHandle(input))
          return (IHandler2<TIn, TOut>) menu;
      }

      return nullptr;
    }
  };
}
