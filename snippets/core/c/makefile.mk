# -----------------------------------------------------------------------------
# core
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/core/_*.c), \
                         $(wildcard $(ROOTDIR)/core/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/core/*.h)
CFLAGS   += -I$(ROOTDIR)
