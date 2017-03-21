#include "catch.hpp"

//#include <Arduino.h>
#include <fact/c_types.h>
#include <fact/string_convert.h>
//#include <fact/menu_invoke.h>
#include <fact/string.h>

namespace experimental2 = FactUtilEmbedded::experimental;

SCENARIO( "Low level string conversion/handling functions", "[string-convert]" )
{
  char buffer[128];

  GIVEN("Converting a buffer from integer")
  {
    int value = 10;

    toString(buffer, value);

    std::string b = buffer;

    REQUIRE(b == "10");
  }
  GIVEN("Converting a buffer from a character")
  {
    char value = 'a';

    toString(buffer, value);

    std::string b = buffer;

    REQUIRE(b == "a");
  }
  GIVEN("Validate a buffer as integer")
  {
    strcpy(buffer, "10");

    PGM_P result = validateString<int>(buffer);

    REQUIRE(result == nullptr);
  }
  GIVEN("Validate a buffer as a malformed integer")
  {
    strcpy(buffer, "10X");

    PGM_P result = validateString<int>(buffer);

    REQUIRE(result != nullptr);
    REQUIRE(result == VALIDATE_FORMAT_ERROR);
  }
  GIVEN("Validate a buffer as float")
  {
    strcpy(buffer, "10.0");

    PGM_P result = validateString<float>(buffer);

    REQUIRE(result == nullptr);
  }
  GIVEN("Validate a buffer as a malformed float")
  {
    strcpy(buffer, "X");

    PGM_P result = validateString<float>(buffer);

    REQUIRE(result != nullptr);
    REQUIRE(result == VALIDATE_FORMAT_ERROR);
  }
  GIVEN("Retrieve a flaat directly from a buffer")
  {
    auto value = fromString<float>("3.3");

    REQUIRE(value == 3.3f);
  }
  GIVEN("Deduce maximum buffer size from type")
  {
      char buffer2[experimental::maxStringLength<float>()];

      REQUIRE(sizeof(buffer2) == 32);
  }
  GIVEN("layer1 string")
  {
      experimental2::layer1::string<6> str = { "HELLO" };
      auto str2 = experimental2::layer1::getString("Hi");

      auto sizetest = experimental2::layer1::size("HI");
      auto sizetest2 = experimental2::layer1::size(buffer);

      auto str3 = experimental2::layer1::getString2("Hi");
      experimental2::layer3::string<> str4(str.getData());
      REQUIRE(str4.getData() == str.getData());
    //exp2::
  }
}
