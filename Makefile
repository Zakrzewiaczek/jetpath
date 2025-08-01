########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings
CC = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = 

# Makefile settings
APPNAME = jetpath
EXT = .cpp
SRCDIR = .
OBJDIR = .
TESTDIR = tests

############## Do not change anything from here downwards! #############
# Only compile main.cpp for the main app
MAIN_SRC = main.cpp
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
DEP = $(MAIN_OBJ:.o=.d)

# Test files
TEST_SRC = $(wildcard $(TESTDIR)/*$(EXT))
TEST_OBJ = $(TEST_SRC:$(TESTDIR)/%.cpp=test_%.o)

# UNIX-based OS variables & settings
RM = rm -f
DELOBJ = $(MAIN_OBJ) $(TEST_OBJ)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the main app (only from main.cpp)
$(APPNAME): $(MAIN_OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Builds and runs tests
test: test_runner
	./test_runner

test_runner: $(TEST_OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ -lgtest -lgtest_main -pthread

# Runs the main application
run: $(APPNAME)
	./$(APPNAME)

# Building rule for main.cpp
%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Building rule for test files
test_%.o: $(TESTDIR)/%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(APPNAME) test_runner *.d

.PHONY: all test run clean