

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

  // want base of ServiceBase to be protected for setState,
  // but (sort of) want it public for event stateUpdated access
  class Service : public IService, public layer3::ServiceBase
  {
  public:
    void doStart();
    void doStop();
    void doRestart();

    Service(const char* name) : layer3::ServiceBase(name) {}

    PGM_P getStatusMessage() const
    {
      // TODO: make sure PGM_P really is a full replacement for FlashStringHelper
      //  and if so, replace flashstringhelper with it throughout Service code
      return (PGM_P) layer3::ServiceBase::getStatusMessage();
    }
  };
}
