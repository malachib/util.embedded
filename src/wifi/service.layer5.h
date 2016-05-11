namespace layer5
{
  class WiFiService : public Service
  {
    // not progmem , just regular const char*
    static const char* _WLAN_SSID;
    static const char* _WLAN_PASS;

    // this one is progmem
    static const char NAME[];

  protected:
    virtual void start() override;

  public:
    WiFiService() : Service(NAME) {}

    void setSSID(const char* ssid, const char* pass)
    {
      _WLAN_SSID = ssid;
      _WLAN_PASS = pass;
    }

    void doStart(const char* ssid, const char* pass)
    {
      setSSID(ssid, pass);
      Service::doStart();
    }
  };
}
