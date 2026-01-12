DIRBIN = bin
DIRSRC = src
DIRTESTS = tests
DIRBUILD = build
DIRLOGS = logs

COMPILERCPP = g++-13
FLAGS = -std=c++23 -O3
# INCLUDELIB = -I./$(DIRSRC)

CPPTESTFILES = $(wildcard $(DIRTESTS)/*.cpp)

TESTFILES = $(patsubst $(DIRTESTS)/%.cpp, %, $(CPPTESTFILES))

LOGFILES = $(patsubst %, $(DIRLOGS)/%.txt, $(TESTFILES))
BINFILES = $(patsubst %, $(DIRBIN)/%.exe, $(TESTFILES))
OBJFILES = $(patsubst %, $(DIRBUILD)/%.o, $(TESTFILES))
DEPFILES = $(patsubst %, $(DIRBUILD)/%.d, $(TESTFILES))

STRDONE = \033[92m - Done:\033[0m
STRTESTED = \033[92m - Tested:\033[0m

.PHONY: all test

all: $(DIRBUILD) $(DIRBIN) $(DIRLOGS) $(BINFILES)

test: all $(LOGFILES)


-include $(DEPFILES)

# dirs
$(DIRBIN):
	@mkdir -p $(DIRBIN)

$(DIRBUILD):
	@mkdir -p $(DIRBUILD)

$(DIRLOGS):
	@mkdir -p $(DIRLOGS)

# debs
$(DIRBUILD)/%.d: $(DIRTESTS)/%.cpp
	@$(COMPILERCPP) -MM -MT $(patsubst $(DIRTESTS)/%.cpp,$(DIRBUILD)/%.o,$<) $< $(FLAGS) > $@
	@$(COMPILERCPP) -MM -MT $(patsubst $(DIRTESTS)/%.cpp,$(DIRBUILD)/%.d,$<) $< $(FLAGS) > $@
	@echo "$(STRDONE) \033[95m$@\033[0m"

# other
$(DIRBUILD)/%.o: $(DIRTESTS)/%.cpp $(DIRBUILD)/%.d
	@$(COMPILERCPP) -c $< -o $@ $(FLAGS)
	@echo "$(STRDONE) \033[95m$@\033[0m"

$(DIRBIN)/%.exe: $(DIRBUILD)/%.o
	@$(COMPILERCPP) $< -o $@ $(FLAGS) $(LIBS)
	@echo "$(STRDONE) \033[96m$@\033[0m"

#test
$(DIRLOGS)/%.txt: $(DIRBIN)/%.exe 
	@echo "\033[95m - Start tested:\033[0m $<"
	@$< | tee -a $@
	@touch $@
	@echo "$(STRTESTED) $<"


