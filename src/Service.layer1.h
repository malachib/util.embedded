namespace layer1
{
  // TODO:
  //   use variadic template to make: static-event-notifier

  //template <bool (*init)(layer1::Service& svc)>
  //class Service;

  //Event2<IService*, char*> stateUpdated;

  class ServiceBase : public ServiceState, public FactUtilEmbedded::Named
  {
  public:
    ServiceBase(PGM_P name) : Named((const __FlashStringHelper*) name)
    {
#ifdef FACT_LIB_STRICT
      setState(Unstarted);
#endif
    }
  };

  template <bool (*init)()>
  class Service : public ServiceBase
  {
  protected:
  public:
    void start()
    {
      setState(Starting);
      if(init())
        setState(Started);
      else
        setState(Error);
    }

    void restart() { start(); }

    Service(PGM_P name) : ServiceBase(name) {}
  };

}
