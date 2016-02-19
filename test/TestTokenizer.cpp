#include "catch.hpp"

#include <Tokenizer.h>
#include <string.h>


SCENARIO( "Tokenizing tests", "[tokenizer]" )
{
  char _input[] = "ABC,DEF,GHI";

  GIVEN("A given tokenizer with 'ABC,DEF,GHI'")
  {
    char* input = _input;
    char buffer[20];

    Tokenizer tokenizer(buffer,",");

    // interesting, these reset "input"
    //WHEN("checking for ABC")
    {
      auto len = tokenizer.parseToken(input);
      input += len;
      std::string token = tokenizer.getBuffer();
      std::string _input = input;
      REQUIRE(len == 4);
      REQUIRE(token == "ABC");
      REQUIRE(_input == "DEF,GHI");
      //printf("\r\ninput = %s", input);
    }

    //WHEN("checking for DEF")
    {
      //printf("\r\ninput = %s", input);
      strcpy(buffer, "DEF");
      tokenizer.reset();
      auto len = tokenizer.parseToken(input);
      input += len;
      std::string _input = input;
      std::string token2 = tokenizer.getBuffer();
      REQUIRE(len == 4);
      REQUIRE(_input == "GHI");
      REQUIRE(token2 == "DEF");
      //printf("\r\ninput = %s", input);
    }

    //WHEN("checking for GHI")
    {
      tokenizer.reset();
      input += tokenizer.parseToken(input);
      std::string token = tokenizer.getBuffer();
      REQUIRE(token == "GHI");
    }
  }
  GIVEN("A given in-place tokenizer with 'ABC, DEF,GHI'")
  {
    char* input = _input;

    TokenizerInPlace tokenizer(input,",");

    std::string token = tokenizer.parseTokenDestructive();
    REQUIRE(token == "ABC");

    token = tokenizer.parseTokenDestructive();
    REQUIRE(token == "DEF");

    token = tokenizer.parseTokenDestructive();
    REQUIRE(token == "GHI");
  }
}
