# -----------------------------------------------------------------------------
# core
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/core/_*.cpp), \
                         $(wildcard $(ROOTDIR)/core/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/core/_*.c), \
                         $(wildcard $(ROOTDIR)/core/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/core/*.hpp) \
			$(wildcard $(ROOTDIR)/core/*.h)

# core/core
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/core/core/_*.cpp), \
                         $(wildcard $(ROOTDIR)/core/core/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/core/core/_*.c), \
                         $(wildcard $(ROOTDIR)/core/core/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/core/core/*.hpp) \
			$(wildcard $(ROOTDIR)/core/core/*.h)

# core/geometry
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/core/geometry/_*.cpp), \
                         $(wildcard $(ROOTDIR)/core/geometry/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/core/geometry/_*.c), \
                         $(wildcard $(ROOTDIR)/core/geometry/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/core/geometry/*.hpp) \
			$(wildcard $(ROOTDIR)/core/geometry/*.h)

CFLAGS   += -I$(ROOTDIR) -I$(ROOTDIR)/core
