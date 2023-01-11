# -----------------------------------------------------------------------------
# Target and project macros
TARGET 	 := test
BINARY   := $(join $(TARGET),.out)
LIBRARY  := lib$(join $(TARGET),.a)

# Source files, include files and search paths
SOURCES  := $(filter-out $(wildcard _*.c), $(wildcard *.c))
INCLUDES := $(wildcard *.h)
CFLAGS   := -I.

# Template makefile.mk
# 	SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/source/_*.c), \
#   	                     $(wildcard $(ROOTDIR)/source/*.c))
# 	INCLUDES += $(wildcard $(ROOTDIR)/source/*.h)
# 	CFLAGS   += -I$(ROOTDIR)/include
#
#	SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/source/_*.cpp), \
#   	                     $(wildcard $(ROOTDIR)/source/*.cpp)) \
#       	    $(filter-out $(wildcard $(ROOTDIR)/source/_*.c), \
#           	             $(wildcard $(ROOTDIR)/source/*.c))
#	INCLUDES += $(wildcard $(ROOTDIR)/include/*.hpp) \
#               $(wildcard $(ROOTDIR)/include/*.h)
#	CFLAGS   += -I$(ROOTDIR)/include
ROOTDIR	 := ..
include $(ROOTDIR)/makefile.mk

# Objects and dependencies
OBJECTS  := $(patsubst %.c,%.o,$(SOURCES))
DEPENDS  := $(patsubst %.c,%.d,$(SOURCES))

# -----------------------------------------------------------------------------
# Compiler settings
AR      := ar rcs
RM      := rm -vf
CP      := cp -vf
WC      := wc
TAR     := tar
AWK     := gawk
ECHO    := echo
INSTALL := install
SHELL	:= bash
UNAME   := $(shell uname -s)

# Darwin kernel flags
ifeq ($(UNAME), Darwin)
CC      := gcc-mp-9

CFLAGS  += -march=native -mavx -Wa,-q
CFLAGS  += -I/opt/local/include -Wall -std=c99
CFLAGS  += -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS  += $(shell /opt/local/bin/pkg-config --cflags glew)
CFLAGS  += $(shell /opt/local/bin/pkg-config --cflags glfw3)

LDFLAGS += -L/opt/local/lib -Wl,-rpath,/opt/local/lib -lm
LDFLAGS += -Wl,-framework,OpenGL -Wl,-framework,Carbon
LDFLAGS += -Wl,-framework,Cocoa -Wl,-framework,CoreVideo -Wl,-framework,IOKit
LDFLAGS += -Wl,-framework,OpenCL
LDFLAGS += $(shell /opt/local/bin/pkg-config --libs glew)
LDFLAGS += $(shell /opt/local/bin/pkg-config --libs glfw3)
endif

# Linux kernel flags
ifeq ($(UNAME), Linux)
CC      := gcc

CFLAGS  += -march=native -mavx
CFLAGS  += -I/usr/include -Wall -std=c99
CFLAGS  += -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS  += $(shell /usr/bin/pkg-config --cflags glew)
CFLAGS  += $(shell /usr/bin/pkg-config --cflags glfw3)

LDFLAGS += -L/usr/lib64 -Wl,-rpath,/usr/lib64 -lm
LDFLAGS += -lGL -lGLU
LDFLAGS += -lOpenCL
LDFLAGS += $(shell /usr/bin/pkg-config --libs glew)
LDFLAGS += $(shell /usr/bin/pkg-config --libs glfw3)
endif

# Enable/disable debug flags
# CFLAGS  += -g -ggdb -O0 -pedantic -fopt-info-vec-optimized
CFLAGS  += -Ofast

# Enable/disable OpenMP flags
CFLAGS  += -fopenmp
LDFLAGS += -fopenmp

# Enable/disable Pthreads flags
# CFLAGS  += -pthread
# LDFLAGS += -pthread

# -----------------------------------------------------------------------------
# Target rules

## help: Show this message.
#	@sed -n 's/^##//p' $(1)
define makehelp
	@$(AWK) \
		'BEGIN { printf("\nusage: make [\033[0;36mtarget\033[0m]\n"); } \
		{ \
			if ($$1 == "##") { \
				printf("\033[0;36m %-16s \033[0m", $$2); \
				for (i=3; i<=NF; i++) printf("%s ", $$i);\
				printf "\n"; \
			} \
		} \
		END { printf "\n"; }' < $(1)
endef

.DEFAULT_GOAL := help
.PHONY: help
help: $(firstword $(MAKEFILE_LIST))
	$(call makehelp,$<)

## all: Build all targets.
.PHONY: all
all: bin lib

## clean: Remove auto generated files.
.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(BINARY) $(LIBRARY)

## bin: Build the binary program.
.PHONY: bin
bin: $(BINARY)

## lib: Build the static library.
.PHONY: lib
lib: $(LIBRARY)

# Binary and static library
$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

$(LIBRARY): $(OBJECTS)
	$(AR) $(LIBRARY) $(OBJECTS)

# Objects and dependencies
define makedep
	$(eval SRCFILE := $(1))
	$(eval DEPFILE := $(2))
	$(eval DEPDIR  := $(3))
	@if [[ "$(DEPDIR)" == "." ]] || [[ "x$(DEPDIR)" == "x" ]]; \
	then \
	$(CC) -MM -MG $(CFLAGS) $(SRCFILE) | sed -e 's#^\(.*\)\.o:#\1.o:#g' > $(DEPFILE); \
	else \
	$(CC) -MM -MG $(CFLAGS) $(SRCFILE) | sed -e 's#^\(.*\)\.o:#$(DEPDIR)/\1.o:#g' > $(DEPFILE); \
	fi;
endef

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(call makedep,$<,$(patsubst %c,%d,$<),$(shell dirname $<))

-include $(DEPENDS)
