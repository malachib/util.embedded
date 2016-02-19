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

    while(!tokenizer.parse(*input++));
    std::string token = tokenizer.getBuffer();
    REQUIRE(token == "ABC");

    tokenizer.reset();
    while(!tokenizer.parse(*input++))
    std::string token = tokenizer.getBuffer();
    REQUIRE(token == "DEF");

    tokenizer.reset();
    while(!tokenizer.parse(*input++))
    std::string token = tokenizer.getBuffer();
    REQUIRE(token == "GHI");
  }
  GIVEN("A given in-place tokenizer with 'ABC, DEF,GHI'")
  {
    char* input = _input;

    TokenizerInPlace tokenizer(input,",");

    char* token = tokenizer.parseTokenDestructive();
    REQUIRE(strcmp(token, "ABC") == 0);
  }
}
