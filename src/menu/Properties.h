#pragma once

#include "../fact/MenuCommand.h"

namespace FactUtilEmbedded
{
  template <class T>
  class PropertyMenu : public MenuCommand
  {
  protected:
    virtual void handleCommand(Parameters p) override
    {
      if(p.count != 1)
      {
        renderParameterError(1, p.count);
        return;
      }

      //fromString<T>(p.parameter[0]);
    }
  };
}
