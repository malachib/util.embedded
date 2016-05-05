namespace layer2
{
  class ServiceBase : public layer1::ServiceBase
  {
  private:
    const __FlashStringHelper* statusMessage = (const __FlashStringHelper*) LightweightService::emptyString;

  //protected:
  // temporarily public as we build things out (init code needs it public)
  public:
    void setStatusMessage(const __FlashStringHelper* statusMessage)
    {
      this->statusMessage = statusMessage;
    }

    const __FlashStringHelper* getStatusMessage() const
    {
      return statusMessage != nullptr ? statusMessage : getStateString();
    }

  public:
    ServiceBase(const char *name) : layer1::ServiceBase(name) {}
  };

  template <bool (*init)(ServiceBase& svc)>
  class Service : public ServiceBase
  {
  public:
    Service(const char* name) : ServiceBase(name) {}

    /*
    void setStatusMessage(const __FlashStringHelper* statusMessage)
    {
      ServiceBase::setStatusMessage(statusMessage);
    }*/

    void start()
    {
      setState(Starting);
      if(init(*this))
        setState(Started);
      else
        setState(Error);
    }
  };
}
