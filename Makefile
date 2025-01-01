NAME 	 = json

CFLAGS   = -std=c11 -Wall -g -fPIC
CXXFLAGS = -std=c++17 -Wall -g -fPIC


ifdef DEBUG
	CFLAGS += -fsanitize=address
	CXXFLAGS += -fsanitize=address
endif

CC=gcc
CXX=g++

# Automated code gerneation. Don't fuck with it 
# unless you know what your doing!
TARGET  = lib$(NAME).so
TEST	= test_$(NAME)
BENCH   = benchmark_$(NAME)

# Yacc file go first because they generat headers
YACSRC = $(patsubst %.y,%.tab.c,$(wildcard src/*.y))
LEXSRC = $(patsubst %.l,%.lex.c,$(wildcard src/*.l))
CXXSRC = $(wildcard src/*.cpp)
CSRC   = $(wildcard src/*.c)

OBJ := $(patsubst %.cpp,build/%.o,$(CXXSRC))
OBJ += $(patsubst %.c,build/%.o,$(CSRC))

# Test code
TESTCXXSRC := $(wildcard test/*.cpp)
TESTCSRC   := $(wildcard test/*.c)
TESTOBJ	   := $(patsubst %.cpp,build/%.o,$(TESTCXXSRC))
TESTOBJ	   += $(patsubst %.c,build/%.o,$(TESTCSRC))

# Benchmark code
BENCHCXXSRC := $(wildcard benchmark/*.cpp)
BENCHCSRC   := $(wildcard benchmark/*.c)
BENCHOBJ    := $(patsubst %.cpp,build/%.o,$(BENCHCXXSRC))
BENCHOBJ    += $(patsubst %.c,build/%.o,$(BENCHCSRC))

# Build targets
BUILDTARGET = build/$(TARGET)
TESTTARGET  = build/$(TEST)
BENCHTARGET = build/$(BENCH)

# Force rebuild of flex and bison files each time
all: $(BUILDTARGET)
# 	rm -f src/*.tab.* src/*.lex.*

test: $(BUILDTARGET) $(TESTTARGET)
	$(TESTTARGET)

benchmark: $(BUILDTARGET) $(BENCHTARGET)
	$(BENCHTARGET)

# Dynamic Build Rules
$(BUILDTARGET) : build $(OBJ)
	$(CXX) $(CXXFLAGS) -fPIC -Lbuild -Isrc $(OBJ) -shared -o $(BUILDTARGET)

$(TESTTARGET): build $(TESTOBJ) $(BUILDTARGET) 
	$(CXX) $(CXXFLAGS) -Lbuild -Isrc -Itest $(TESTOBJ) -lgtest -lpthread -lglog -ljson  -o $(TESTTARGET) -Wl,-rpath,build

$(BENCHTARGET): build $(BENCHOBJ)
	$(CXX) $(CXXFLAGS) -Lbuild -Iback -Isrc -Ibenchmark $(BENCHOBJ) -ljson -lbenchmark -lpthread -o $(BENCHTARGET) -Wl,-rpath,build


cmd_example: $(BUILDTARGET)
	g++ -std=c++17 -Wall -Isrc -c example/cmd_example.cpp -o cmd_example.o
	g++ -std=c++17 -Lbuild -ljsmn -o cmd_example cmd_example.o -Wl,-rpath,build
	rm -f cmd_example.o

jsondump: $(BUILDTARGET)
	g++ -std=c++17 -g -Wall -Isrc -c example/jsondump.cpp -o jsondump.o
	g++ -std=c++17 -g -Lbuild -ljsmn -o jsondump jsondump.o -Wl,-rpath,build
	rm -f jsondump.o

simple: $(BUILDTARGET)
	g++ -std=c++17 -g -Wall -Isrc -c example/simple.cpp -o simple.o
	g++ -std=c++17 -g -Lbuild -ljsmn -o simple simple.o -Wl,-rpath,build
	rm -f simple.o

main: $(BUILDTARGET)
	gcc $(CFLAGS) -Isrc -c main/main.c  -o build/main.o
	gcc $(CFLAGS) -Lbuild build/main.o -ljson -o build/main -Wl,-rpath,build

build:
	mkdir -p build/src
	mkdir -p build/test
	mkdir -p build/benchmark

src/%.tab.c: src/%.y
	bison -d $< -o $@

src/%.lex.c: src/%.l
	flex -o $@ $<

build/%.o: %.c
	$(CC) -c $(CFLAGS) -Lbuild -Isrc -Ibenchmark $< -o $@

build/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -Lbuild -Isrc -Ibenchmark -Iback $< -o $@

clean:
	rm -rf build src/*.tab.h src/*.tab.c src/*.lex.c

.PRECIOUS: src/json.tab.c src/json.lex.c

