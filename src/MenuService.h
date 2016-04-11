#pragma once

#include "Service.h"
#include "fact/Menu.h"
#include "fact/rpc.h"

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC

namespace FactUtilEmbedded
{

class MenuService : public MenuCommand
{
  Service& service;

protected:
  virtual void handleCommand(Parameters p) override;

public:
  MenuService(Service& service) : service(service) {}
};


template <uint8_t TSize, uint8_t TLen, rpc::CallQueue<TSize, TLen>& cq>
//template <rpc::CallQueue<,> _cq>
// TODO: figure out how to get above to work so that we don't have to
// drag around TCallQueue ptr
//template <class TCallQueue>
class MenuServiceRPC : public MenuCommand
{
  Service& service;
  //TCallQueue& cq;

protected:
  virtual void handleCommand(Parameters p) override
  {
    auto callHolder = rpc::CallHolderFactory::create(&Service::restart, &service);
    cq.put(callHolder, sizeof(callHolder));
  }

public:
  MenuServiceRPC(Service& service) :
    //, TCallQueue& cq) :
    service(service)
    //, cq(cq)
    {}
};

}

#endif
