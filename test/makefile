CC = g++
ODIR = ../obj
SDIR = .
OUT = test
DEFINE = -DNODUINO=1 -DDEBUG=1
INCLUDE = ../ext/Catch/include
INCLUDE2 = ../src
INCLUDE_NODUINO = noduino
INC = -I$(INCLUDE) -I$(INCLUDE2) -I$(INCLUDE_NODUINO)
LIB = -L$(ODIR)
CFLAGS = --std=c++11 -g
CC_DEBUG = -c
#CC_DEBUG = -g -c

_OBJS = Console.o EventManager.o statemachine.o Handler.o taskmanager.o lib_c.o \
	lib.o LinkedList.o Math.o Menu.o MenuService.o Service.o Arduino.o Main.o \
	Tokenizer.o \
	TestTokenizer.o TestGNSS.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $^

# test app
$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(INC) $(DEFINE) $(CC_DEBUG) $< $(CFLAGS) -o $@

# catch lib
$(ODIR)/%.o: $(INCLUDE)/%.cpp
	$(CC) $(INC) $(DEFINE) $(CC_DEBUG) $< $(CFLAGS) -o $@

# util.embedded lib
$(ODIR)/%.o: $(INCLUDE2)/%.cpp
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
