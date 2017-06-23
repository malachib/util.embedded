#include "catch.hpp"
#include "experimental/Template.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental;

SCENARIO( "Template expansion code", "[template-xpand]" )
{
    GIVEN("Test 1")
    {
        StreamPipeline pipeline_in;
        char buf1[] = "hello";
        char buf2[] = "world";
        char buf3[] = "!!";

        pipeline_in.write(buf1, 6);
        pipeline_in.write(buf2, 6);
        pipeline_in.write(buf3, 3);

        auto _buf1 = pipeline_in.read();
        auto _buf2 = pipeline_in.read();
        auto _buf3 = pipeline_in.read();

        std::string __buf1((char*)_buf1.getData());
        std::string __buf2((char*)_buf2.getData());
        std::string __buf3((char*)_buf3.getData());

        REQUIRE(__buf1 == buf1);
        REQUIRE(__buf2 == buf2);
        REQUIRE(__buf3 == buf3);
    }
}
