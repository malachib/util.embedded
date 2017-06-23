CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project("util.embedded.tests")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-c)
ADD_DEFINITIONS(-g)

# Because otherwise it defaults to Arduino mode, which we don't want for POSIX unit tests
ADD_DEFINITIONS(-DFEATURE_IOSTREAM)
ADD_DEFINITIONS(-D__POSIX__)

# Because we still are using sloppy NODUINO flag
# ADD_DEFINITIONS(-DNODUINO)
# Because we need to expose some private/protected stuff so that unit tests can test them
ADD_DEFINITIONS(-DUNIT_TEST)
# Because my 3/4 baked event system has unit tests using the special variable-argument feature
ADD_DEFINITIONS(-DEVENT_FEATURE_VA)

configure_file("data/iostream.test.txt" "iostream.test.txt" COPYONLY)

INCLUDE_DIRECTORIES("../ext/Catch/include")

INCLUDE_DIRECTORIES("../src")

file(GLOB_RECURSE util_embedded_source "../src/*.cpp"
    "../src/*.h" "../src/*.hpp" "../src/fact/iostream"
    )
# this line in above glob doesnt seem to make a difference for linking
#"../src/fact/*.h" "../src/menu/*.h"

add_library(util_embedded_lib ${util_embedded_source})

FILE(GLOB source_main "*.cpp" "noduino/*")

LIST(REMOVE_ITEM source_main "${PROJECT_SOURCE_DIR}/noduino/ncursesStream.cpp")

#message("source_main = ${source_main}")

ADD_EXECUTABLE(${PROJECT_NAME} ${source_main})

# following line doesn't seem to make a difference for linking
TARGET_LINK_LIBRARIES(${PROJECT_NAME} util_embedded_lib)
