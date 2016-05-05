

namespace layer5
{
  // TODO: work out whether start() is the exposed function for the consumer
  // or whether it's the implementing function for the implementor.  Needs
  // an associated function, doStart
  class IService
  {
  protected:
    virtual void start() = 0;
    virtual void stop() {};
    virtual void pause() {};
  };

  class Service : public IService, public layer3::ServiceBase
  {
  public:
    void doStart();
    void doStop();
    void doRestart();

    Service(const char* name) : layer3::ServiceBase(name) {}
  };
}
