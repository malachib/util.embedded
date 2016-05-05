namespace layer3
{
  class ServiceBase : public layer2::ServiceBase
  {
  public:
    events::Event<ServiceBase*> stateUpdated;
    events::Event<ServiceBase*, const char*> statusUpdated;

    void setState(State state)
    {
      ServiceState::setState(state);
      stateUpdated(this);
    }
    
    void setStatusMessage(PGM_P msg, const char* extendedMessage = nullptr)
    {
      layer2::ServiceBase::setStatusMessage(msg);
      statusUpdated(this, extendedMessage);
    }

    ServiceBase(const char* name) :
      layer2::ServiceBase(name) {}
  };

  class Service : public ServiceBase
  {
    typedef bool (*startService)(Service& svc);

  protected:
    const startService startFunc;

  public:
    Service(const char* name, startService startFunc) :
      ServiceBase(name), startFunc(startFunc)
    {
    }

    void start()
    {
      setState(Starting);
      if(startFunc(*this))
        setState(Started);
      else
        setState(Error);
    }
  };
}
