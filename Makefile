BUILDDIR ?= build
CFG      ?= debug
NAME     ?= c4
SRCDIR   ?= src

all:

-include config/$(CFG).cfg

# Be verbose about the build.
Q ?= @

BINDIR := $(BUILDDIR)/$(CFG)
BIN    := $(BINDIR)/$(NAME)
SRC    := $(sort $(wildcard $(SRCDIR)/*.cpp))
OBJ    := $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEP    := $(OBJ:%.o=%.d)

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

DUMMY := $(shell mkdir -p $(sort $(dir $(OBJ))))

.PHONY: all clean

all: $(BIN)

-include $(DEP)

clean:
	@echo "===> CLEAN"
	$(Q)rm -fr $(BINDIR)

$(BIN): $(OBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BIN) $(OBJ) $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<
