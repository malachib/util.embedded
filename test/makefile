ifndef CXX
CXX = g++
endif
CC = $(CXX)
ODIR = ../obj
SDIR = .
OUT = test
DEFINE = -DNODUINO=1 -DDEBUG=1
INCLUDE = ../ext/Catch/include
INCLUDE_LIB = ../src
INCLUDE_NODUINO = noduino
INC = -I$(INCLUDE) -I$(INCLUDE_LIB) -I$(INCLUDE_NODUINO)
LIB = -L$(ODIR)
CFLAGS = --std=gnu++11 -g -DUNIT_TEST
CC_DEBUG = -c
#CC_DEBUG = -g -c

_SRC_INCLUDE = $(wildcard $(INCLUDE_LIB)/*.cpp)
SRC_INCLUDE = $(patsubst $(INCLUDE_LIB)/%.cpp,%,$(_SRC_INCLUDE))
TEST_CPP_FILES = $(wildcard $(SDIR)/*.cpp)
TEST_OBJ_TARGETS = $(patsubst $(SDIR)/%.cpp,%,$(TEST_CPP_FILES))

_OBJS = $(SRC_INCLUDE) $(TEST_OBJ_TARGETS) Arduino
OBJS = $(patsubst %,$(ODIR)/%.o,$(_OBJS))

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $^

# test app
$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(INC) $(DEFINE) $(CC_DEBUG) $< $(CFLAGS) -o $@

# catch lib
$(ODIR)/%.o: $(INCLUDE)/%.cpp
	$(CC) $(INC) $(DEFINE) $(CC_DEBUG) $< $(CFLAGS) -o $@

# util.embedded lib
$(ODIR)/%.o: $(INCLUDE_LIB)/%.cpp
	$(CC) $(DEFINE) -I$(INCLUDE_NODUINO) $(CC_DEBUG) $< $(CFLAGS) -o $@

# noduino pseudo-lib
$(ODIR)/%.o: $(INCLUDE_NODUINO)/%.cpp
	$(CC) $(DEFINE) $(CC_DEBUG) $< $(CFLAGS) -o $@

#all: $(OBJS)

# ensure ODIR exists
$(OBJS): | $(ODIR)

# ensure ODIR exists
$(ODIR):
	mkdir -p $(ODIR)

clean:
	rm $(OUT) $(ODIR)/*.o
