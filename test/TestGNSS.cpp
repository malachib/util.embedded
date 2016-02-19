#include "catch.hpp"

#include <Tokenizer.h>
#include <string.h>

#include <driver/gnss.h>

using namespace driver;

class Dummy_GNSS_raw : public IGNSS_raw
{
public:
  VIRTUAL MetaData getMetaData() OVERRIDE
  {
    MetaData md;
    return md;
  }
  VIRTUAL uint16_t getContextSize() OVERRIDE { return 0; }
  VIRTUAL bool getGNSS(IGNSS_raw_token_callback callback, void* context = NULL) OVERRIDE;
};

bool Dummy_GNSS_raw::getGNSS(IGNSS_raw_token_callback callback, void* context)
{
  char buffer[] = "1,2,12.0,14.0,XZ";
  TokenizerInPlace tokenizer(buffer,",");
  for(int i = 0; i < 5; i++)
  {
    char* token = tokenizer.parseTokenDestructive();

    callback((GnssFields)i, token, context);
  }
  return true;
}

SCENARIO( "GNSS synthetic tests", "[gnss]" )
{
  Dummy_GNSS_raw gnss_raw;

  GIVEN("A given GPS input of '1,2,12.0,14.0'")
  {
    bool fulfilled[] = { false, false, false, false, false };
    gnss_raw.getGNSS([](GnssFields field, char* _token, void* context) -> bool
    {
      std::string token = _token;
      auto fulfilled = (bool*) context;

      fulfilled[(int)field] = true;

      switch((int)field)
      {
        case 0:
          REQUIRE(token == "1");
          break;

        case 1:
          REQUIRE(token == "2");
          break;

        case 2:
          REQUIRE(token == "12.0");
          break;

        case 3:
          REQUIRE(token == "14.0");
          break;

        case 4:
          REQUIRE(token == "XZ");
          break;

        default:
          REQUIRE((int)field <= 4);
          break;
        }

        return true;
      }
    , fulfilled);

    for(int i = 0; i < 5; i++)
      REQUIRE(fulfilled[i] == true);
  }
}
