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
  VIRTUAL bool getGNSS(token_callback callback, void* context = NULL) OVERRIDE;
};


bool getGNSS_helper_d(IGNSS_raw::token_callback callback, char* input, GnssFields* fieldOrder, uint8_t fieldCount, void* context)
{
  TokenizerInPlace tokenizer(input,",");
  for(int i = 0; i < fieldCount; i++)
  {
    char* token = tokenizer.parseTokenDestructive();

    if(!callback(fieldOrder[i], token, context))
      return false;
  }
  return true;

}

bool getGNSS_helper(IGNSS_raw::token_callback callback, char* input, GnssFields* fieldOrder, uint8_t fieldCount, void* context)
{
  char buffer[20];

  Tokenizer tokenizer(buffer,",");

  for(int i = 0; i < fieldCount; i++)
  {
    input += tokenizer.parseToken(input);

    char* token = tokenizer.getBuffer();

    if(!callback(fieldOrder[i], token, context))
      return false;

    tokenizer.reset();
  }
  return true;

}

GnssFields fieldOrder[] =
{
  GNSS_LATITUDE, GNSS_LONGITUDE, GNSS_SPEED, GNSS_HEADING, GNSS_TIMESTAMP,
  // skip altitude (#5) to test for false positives
  GNSS_LATP, GNSS_LONGP
};
char buffer[] = "1,2,12.0,14.0,XZ,,ABC";


bool Dummy_GNSS_raw::getGNSS(IGNSS_raw::token_callback callback, void* context)
{
  return getGNSS_helper_d(callback, buffer, fieldOrder,
    sizeof(fieldOrder) / sizeof(GnssFields), context );
  TokenizerInPlace tokenizer(buffer,",");
  for(int i = 0; i < 5; i++)
  {
    char* token = tokenizer.parseTokenDestructive();

    callback((GnssFields)i, token, context);
  }
  return true;
}

bool token_callback(GnssFields field, char* _token, void* context)
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

    case 6:
      REQUIRE(token == "");
      break;

    case 7:
      REQUIRE(token == "ABC");
      break;

    default:
      REQUIRE((int)field <= 4);
      break;
  }

  return true;
}

SCENARIO( "GNSS synthetic tests", "[gnss]" )
{
  Dummy_GNSS_raw gnss_raw;

  GIVEN("A given GPS input of '1,2,12.0,14.0,XZ,,ABC'")
  {
    bool fulfilled[] = { false, false, false, false, false, false, false, false };

    /*
    WHEN("doing it non-destructively")
    {
      getGNSS_helper(token_callback, buffer, fieldOrder,
        sizeof(fieldOrder) / sizeof(GnssFields), fulfilled);

      for(int i = 0; i < 7; i++)
      {
        // we specifically don't get that field
        if(i == 5) continue;
        INFO("Checking: " << i);
        REQUIRE(fulfilled[i] == true);
      }
    } */
    
    WHEN("doing it destructively")
    {
      gnss_raw.getGNSS(token_callback, fulfilled);

      for(int i = 0; i < 7; i++)
      {
        // we specifically don't get that field
        if(i == 5) continue;
        INFO("Checking: " << i);
        REQUIRE(fulfilled[i] == true);
      }
    }

  }
}
