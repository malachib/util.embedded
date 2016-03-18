ifndef CXX
CXX = g++
endif
CC = $(CXX)
ODIR = obj
SDIR = .
OUT = test
DEFINE = -DNODUINO=1 -DDEBUG=1
#-DCONSOLE_STREAM_IN=cin -DCONSOLE_STREAM_OUT=cout
INCLUDE = ../../ext/Catch/include
INCLUDE_LIB = ../../src
INCLUDE_NODUINO = ../noduino
INC = -I$(INCLUDE) -I$(INCLUDE_LIB) -I$(INCLUDE_NODUINO)
LIB = -L$(ODIR)
CFLAGS = --std=gnu++11 -g -DUNIT_TEST -DNCURSES
CC_DEBUG = -c
#CC_DEBUG = -g -c

_SRC_INCLUDE = $(wildcard $(INCLUDE_LIB)/*.cpp)
SRC_INCLUDE = $(patsubst $(INCLUDE_LIB)/%.cpp,%,$(_SRC_INCLUDE))

#$(info $$_SRC_INCLUDE is [${_SRC_INCLUDE}])

#_OBJS = Console.o EventManager.o statemachine.o Handler.o taskmanager.o lib_c.o \
#	lib.o LinkedList.o Math.o Menu.o MenuService.o Service.o Arduino.o ncursesStream.o \
#	Tokenizer.o \
#	main.o
_OBJS = $(SRC_INCLUDE) Arduino ncursesStream main
OBJS = $(patsubst %,$(ODIR)/%.o,$(_OBJS))

$(OUT): $(OBJS)
	$(CC) -lncurses -o $(OUT) $^

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
