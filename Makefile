BUILDDIR ?= build
CFG      ?= debug
NAME     ?= c4
SRCDIR   ?= src

# Test stuff

TESTBUILDDIR	?= build_test
TESTNAME		?= runner
TESTSRCDIR		?= test

all:

-include config/$(CFG).cfg

# Be verbose about the build.
Q ?= @

BINDIR := $(BUILDDIR)/$(CFG)
BIN    := $(BINDIR)/$(NAME)
SRC    := $(sort $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/**/**/*.cpp $(SRCDIR)/**/*.cpp))
OBJ    := $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEP    := $(OBJ:%.o=%.d)

# Test stuff

TESTBINDIR	:= $(TESTBUILDDIR)
TESTBIN		:= $(TESTBINDIR)/$(TESTNAME)
TESTSRC		:= $(sort $(wildcard $(TESTSRCDIR)/*.cpp))
TESTOBJ		:= $(TESTSRC:$(TESTSRCDIR)/%.cpp=$(TESTBINDIR)/%.o) $(filter-out $(BINDIR)/main.o,$(OBJ))
TESTDEP		:= $(TESTOBJ:%.o=%.d)

# Try to locate llvm-config, a tool that produces command line flags for the
# build process.
ifneq ("$(wildcard llvm/install/bin/llvm-config)","")
	LLVM_CONFIG  ?= llvm/install/bin/llvm-config
else
	LLVM_CONFIG  ?= llvm-config
endif

# Obtain the LLVM build flags if we found llvm-config.
ifeq (,$(wildcard $(LLVM_CONFIG)))
	LLVM_CFLAGS  :=
	LLVM_LDFLAGS :=
else
	LLVM_CFLAGS  := $(shell $(LLVM_CONFIG) --cppflags)
	LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags --libs --system-libs)
endif

CFLAGS   := $(LLVM_CFLAGS) -Wall -W $(CFLAGS)
CXXFLAGS += $(CFLAGS) -std=c++17
LDFLAGS  += $(LLVM_LDFLAGS)

# Test stuff

TESTCXXFLAGS	:= $(CXXFLAGS) -I $(SRCDIR)
TESTLDFLAGS		:= $(LDFLAGS) -lcppunit

DUMMY := $(shell mkdir -p $(sort $(dir $(OBJ)) $(dir $(TESTOBJ))))

.PHONY: all clean

all: $(BIN)

-include $(DEP)

test: $(TESTBIN)
	$(TESTBIN)

-include $(TESTDEP)

clean:
	@echo "===> CLEAN"
	$(Q)rm -fr $(BINDIR)
	$(Q)rm -fr $(TESTBINDIR)

$(BIN): $(OBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BIN) $(OBJ) $(LDFLAGS)

$(TESTBIN): $(TESTOBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(TESTBIN) $(TESTOBJ) $(TESTLDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

$(TESTBINDIR)/%.o: $(TESTSRCDIR)/%.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(TESTCXXFLAGS) -MMD -c -o $@ $<
