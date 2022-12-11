# -----------------------------------------------------------------------------
# ds
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ds/array/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ds/array/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/ds/array/_*.c), \
                         $(wildcard $(ROOTDIR)/ds/array/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ds/array/*.hpp) \
			$(wildcard $(ROOTDIR)/ds/array/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ds/linked/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ds/linked/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/ds/linked/_*.c), \
                         $(wildcard $(ROOTDIR)/ds/linked/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ds/linked/*.hpp) \
			$(wildcard $(ROOTDIR)/ds/linked/*.h)

CFLAGS   += -I$(ROOTDIR) -I$(ROOTDIR)/ds

# -----------------------------------------------------------------------------
# algo
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/algo/array/_*.cpp), \
                         $(wildcard $(ROOTDIR)/algo/array/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/algo/array/_*.c), \
                         $(wildcard $(ROOTDIR)/algo/array/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/algo/array/*.hpp) \
			$(wildcard $(ROOTDIR)/algo/array/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/algo/search/_*.cpp), \
                         $(wildcard $(ROOTDIR)/algo/search/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/algo/search/_*.c), \
                         $(wildcard $(ROOTDIR)/algo/search/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/algo/search/*.hpp) \
			$(wildcard $(ROOTDIR)/algo/search/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/algo/sort/_*.cpp), \
                         $(wildcard $(ROOTDIR)/algo/sort/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/algo/sort/_*.c), \
                         $(wildcard $(ROOTDIR)/algo/sort/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/algo/sort/*.hpp) \
			$(wildcard $(ROOTDIR)/algo/sort/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/algo/strings/_*.cpp), \
                         $(wildcard $(ROOTDIR)/algo/strings/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/algo/strings/_*.c), \
                         $(wildcard $(ROOTDIR)/algo/strings/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/algo/strings/*.hpp) \
			$(wildcard $(ROOTDIR)/algo/strings/*.h)

CFLAGS   += -I$(ROOTDIR) -I$(ROOTDIR)/algo

# -----------------------------------------------------------------------------
# numeric
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/numeric/array/_*.cpp), \
                         $(wildcard $(ROOTDIR)/numeric/array/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/numeric/array/_*.c), \
                         $(wildcard $(ROOTDIR)/numeric/array/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/numeric/array/*.hpp) \
			$(wildcard $(ROOTDIR)/numeric/array/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/numeric/histogram/_*.cpp), \
                         $(wildcard $(ROOTDIR)/numeric/histogram/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/numeric/histogram/_*.c), \
                         $(wildcard $(ROOTDIR)/numeric/histogram/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/numeric/histogram/*.hpp) \
			$(wildcard $(ROOTDIR)/numeric/histogram/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/numeric/linalg/_*.cpp), \
                         $(wildcard $(ROOTDIR)/numeric/linalg/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/numeric/linalg/_*.c), \
                         $(wildcard $(ROOTDIR)/numeric/linalg/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/numeric/linalg/*.hpp) \
			$(wildcard $(ROOTDIR)/numeric/linalg/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/numeric/ode/_*.cpp), \
                         $(wildcard $(ROOTDIR)/numeric/ode/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/numeric/ode/_*.c), \
                         $(wildcard $(ROOTDIR)/numeric/ode/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/numeric/ode/*.hpp) \
			$(wildcard $(ROOTDIR)/numeric/ode/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/numeric/sde/_*.cpp), \
                         $(wildcard $(ROOTDIR)/numeric/sde/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/numeric/sde/_*.c), \
                         $(wildcard $(ROOTDIR)/numeric/sde/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/numeric/sde/*.hpp) \
			$(wildcard $(ROOTDIR)/numeric/sde/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/numeric/transform/_*.cpp), \
                         $(wildcard $(ROOTDIR)/numeric/transform/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/numeric/transform/_*.c), \
                         $(wildcard $(ROOTDIR)/numeric/transform/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/numeric/transform/*.hpp) \
			$(wildcard $(ROOTDIR)/numeric/transform/*.h)

CFLAGS   += -I$(ROOTDIR) -I$(ROOTDIR)/numeric
