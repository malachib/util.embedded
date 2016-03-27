#include "catch.hpp"

#include <Tokenizer.h>
#include <string.h>

#include <driver/gnss.h>
#define FACT_VE defined(DRIVER_FEATURE_VTABLE)
#include <fact/virtual_def.h>

using namespace driver;

class Dummy_GNSS_raw : public IGNSS_raw
{
  char* inputBuffer;

public:
  VIRTUAL MetaData getMetaData() OVERRIDE
  {
    MetaData md;
    return md;
  }
  VIRTUAL uint16_t getContextSize() OVERRIDE { return 0; }
  VIRTUAL bool getGNSS(token_callback callback, void* context = NULL) OVERRIDE;

  void setInput(char* inputBuffer) { this->inputBuffer = inputBuffer; }
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

bool getGNSS_helper(IGNSS_raw::token_callback callback, char* input, char* buffer, GnssFields* fieldOrder, uint8_t fieldCount, void* context)
{
  Tokenizer tokenizer(buffer,",");

  for(int i = 0; i < fieldCount; i++)
  {
    INFO("Got input: " << input);

    input += tokenizer.parseToken(input);

    char* token = tokenizer.getBuffer();

    INFO("Got token: " << token);

    if(!callback(fieldOrder[i], token, context))
      return false;

    if(i == fieldCount - 1) {  REQUIRE(*input == 0); }

    tokenizer.reset();
  }
  return true;

}

GnssFields testFieldOrder[] =
{
  GNSS_LATITUDE, GNSS_LONGITUDE, GNSS_SPEED, GNSS_HEADING, GNSS_TIMESTAMP,
  // skip altitude (#5) to test for false positives
  GNSS_LATP, GNSS_LONGP
};

GnssFields //PROGMEM
SIM808_fieldOrder[] =
{
  GNSS_STATUS_RUN,
  GNSS_STATUS_FIX,
  GNSS_TIMESTAMP,
  GNSS_LATITUDE,
  GNSS_LONGITUDE,
  GNSS_ALTITUDE,
  GNSS_SPEED,
  GNSS_HEADING
};


#define INPUT_BUFFER "1,2,12.0,14.0,XZ,,ABC"
char inputBuffer[] = INPUT_BUFFER;

#define INPUT_BUFFER2 "+CGNSINF: 1,1,20160219110053.000,34.098948,-118.138393,166.100,0.93,93.4,1,,0.8,1.1,0.8,,12,11,,,39,,"
char inputBuffer2[] = INPUT_BUFFER2;

bool Dummy_GNSS_raw::getGNSS(IGNSS_raw::token_callback callback, void* context)
{
  return getGNSS_helper_d(callback, inputBuffer, testFieldOrder,
    sizeof(testFieldOrder) / sizeof(GnssFields), context );
  TokenizerInPlace tokenizer(inputBuffer,",");
  for(int i = 0; i < 5; i++)
  {
    char* token = tokenizer.parseTokenDestructive();

    callback((GnssFields)i, token, context);
  }
  return true;
}

struct Context
{
  // inputBuffer shouldn't live in this callback context.
  // it would be more an instance field or a IGNSS_raw-level context
  //char* inputBuffer;

  // fulfilled is an array representing which GNSS fields have shown up
  // it's mapped directly to GNSS id, so for example, GNSS_ALTITUDE = 5
  // but for these tests we don't use GNSS_ALTITUDE, therefore fulfilled[5]
  // will remain as 'false'
  bool* fulfilled;
};

bool token_callback(GnssFields field, char* _token, void* context)
{
  std::string token = _token;
  auto ctx = (Context*) context;
  //auto fulfilled = (bool*) context;

  ctx->fulfilled[(int)field] = true;

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


bool token_callback2(GnssFields field, char* _token, void* context)
{
  std::string token = _token;

  switch(field)
  {
    case GNSS_STATUS_RUN:
      printf("\r\nStatus run %s", _token);
      break;

    case GNSS_STATUS_FIX:
      printf("\r\nStatus fix %s", _token);
      REQUIRE(token == "1");
      break;

    case GNSS_LATITUDE:
      printf("\r\nLatitude: %s", _token);
      REQUIRE(token == "34.098948");
      break;

    case GNSS_LONGITUDE:
      printf("\r\nLongitude: %s", _token);
      REQUIRE(token == "-118.138393");
      break;

  }
  return true;
}

SCENARIO( "GNSS synthetic tests", "[gnss]" )
{
  Dummy_GNSS_raw gnss_raw;

  GIVEN("A given GPS input of '1,2,12.0,14.0,XZ,,ABC'")
  {
    gnss_raw.setInput(inputBuffer);

    bool fulfilled[] = { false, false, false, false, false, false, false, false };

    Context context;

    //context.inputBuffer = inputBuffer;
    context.fulfilled = fulfilled;

    WHEN("doing it non-destructively")
    {
      char tempBuffer[20];

      getGNSS_helper(token_callback, inputBuffer, tempBuffer, testFieldOrder,
        sizeof(testFieldOrder) / sizeof(GnssFields), &context);

      for(int i = 0; i < 7; i++)
      {
        // we specifically don't get that field
        if(i == 5) continue;
        INFO("GNSS field: " << i);
        REQUIRE(context.fulfilled[i] == true);
      }
    }

    WHEN("doing it destructively")
    {
      gnss_raw.getGNSS(token_callback, &context);

      for(int i = 0; i < 7; i++)
      {
        // we specifically don't get that field
        if(i == 5) continue;
        INFO("GNSS field: " << i);
        REQUIRE(context.fulfilled[i] == true);
      }
    }

    // doesn't work since inputBuffer has indeed been changed
    //REQUIRE(std::string(inputBuffer) == INPUT_BUFFER);
    //REQUIRE(std::string(context.inputBuffer) == INPUT_BUFFER);
  }
  GIVEN("A GPS input of: " INPUT_BUFFER2)
  {
    char *p = strstr(inputBuffer2, F("SINF"));

    REQUIRE(p);

    p+=6;

    //gnss_raw.setInput(p);

    INFO("modified input buffer = " << p);

    getGNSS_helper_d(token_callback2, p, SIM808_fieldOrder,
      sizeof(SIM808_fieldOrder) / sizeof(GnssFields), NULL );
    //gnss_raw.getGNSS(token_callback2, NULL);
  }
}
