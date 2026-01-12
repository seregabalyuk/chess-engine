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

# grpc
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
GRPC_CFLAGS = `pkg-config --cflags protobuf grpc++`
GRPC_LIBS = `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -ldl

SERVER_EXE = $(DIRBIN)/server.exe
PROTO_GENERATED = $(DIRBUILD)/chess.pb.cc $(DIRBUILD)/chess.grpc.pb.cc
PROTO_OBJS = $(DIRBUILD)/chess.pb.o $(DIRBUILD)/chess.grpc.pb.o
SERVER_OBJS = $(DIRBUILD)/main.o $(PROTO_OBJS)

.PHONY: all test dev

all: $(DIRBUILD) $(DIRBIN) $(DIRLOGS) $(BINFILES)

test: all $(LOGFILES)

dev: $(SERVER_EXE)
	$(SERVER_EXE)


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

# grpc rules
$(DIRBUILD)/chess.pb.cc $(DIRBUILD)/chess.grpc.pb.cc: protos/chess.proto | $(DIRBUILD)
	@$(PROTOC) --grpc_out=$(DIRBUILD) --cpp_out=$(DIRBUILD) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) -I protos $<
	@echo "$(STRDONE) \033[95mProtobuf generation\033[0m"

$(DIRBUILD)/chess.pb.o: $(DIRBUILD)/chess.pb.cc
	@$(COMPILERCPP) -c $< -o $@ $(FLAGS) $(GRPC_CFLAGS)
	@echo "$(STRDONE) \033[95m$@\033[0m"

$(DIRBUILD)/chess.grpc.pb.o: $(DIRBUILD)/chess.grpc.pb.cc
	@$(COMPILERCPP) -c $< -o $@ $(FLAGS) $(GRPC_CFLAGS)
	@echo "$(STRDONE) \033[95m$@\033[0m"

$(DIRBUILD)/main.o: src/main.cpp $(DIRBUILD)/chess.grpc.pb.cc
	@$(COMPILERCPP) -c src/main.cpp -o $@ $(FLAGS) $(GRPC_CFLAGS) -I$(DIRBUILD) -I$(DIRSRC)
	@echo "$(STRDONE) \033[95m$@\033[0m"

$(SERVER_EXE): $(SERVER_OBJS)
	@$(COMPILERCPP) $^ -o $@ $(FLAGS) $(GRPC_LIBS)
	@echo "$(STRDONE) \033[96m$@\033[0m"
