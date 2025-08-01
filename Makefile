########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = 

# Makefile settings - Can be customized.
APPNAME = jetpath
EXT = .cpp
SRCDIR = .
OBJDIR = .
TESTDIR = tests

############## Do not change anything from here downwards! #############
# Only compile main.cpp for the main app (exclude test files)
MAIN_SRC = main.cpp
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
DEP = $(MAIN_OBJ:.o=.d)

# Test files
TEST_SRC = $(wildcard $(TESTDIR)/*$(EXT))
TEST_OBJ = $(TEST_SRC:$(TESTDIR)/%.cpp=test_%.o)

# UNIX-based OS variables & settings
RM = rm -f
DELOBJ = $(MAIN_OBJ) $(TEST_OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe

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

# Creates the dependency rules
%.d: %.cpp
	@$(CC) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for main.cpp
%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Building rule for test files
test_%.o: $(TESTDIR)/%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME) test_runner *.d
########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = 

# Makefile settings - Can be customized.
APPNAME = myapp
EXT = .cpp
SRCDIR = .
OBJDIR = .

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)
# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(DELOBJ) $(DEP) $(APPNAME)$(EXE) test_runner$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)

.PHONY: all test run

test_all:
	@cd tests && \
	for test_file in *.cpp; do \
	    echo "Testing $$test_file"; \
	    g++ -std=c++17 -Wall "$$test_file" -lgtest -lgtest_main -pthread -o "$${test_file%.cpp}_runner"; \
	    "./$${test_file%.cpp}_runner"; \
	done

.PHONY: test_all