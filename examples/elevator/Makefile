#
# Make sure to use a compiler which implements all c++11 features!
#
#CROSS = /opt/toolchain/x86_64-pc-linux-gnu-gcc-4.7.0/bin/x86_64-pc-linux-gnu-

PROJECT      = elevator

#FLAGS        = -DDEBUG

# HINT: g++ -Q -O2 --help=optimizers
OPTIMIZER    = -Os

CC           = $(CROSS)gcc
CXX          = $(CROSS)g++
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = $(CROSS)g++
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
#SIZE         = $(CROSS)size -B -d
SIZE         = size -B -d
RM           = rm -f
RM_R         = rm -rf
CP           = cp
MKDIR_P      = mkdir -p
DOXYGEN      = doxygen


SRC_DIRS     = .
INCLUDE      = -I ../../include

SRCS         = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS         = $(SRCS:.cpp=.o)
DEPENDS      = $(OBJS:.o=.d)

EXE          = $(PROJECT)
MAP          = $(PROJECT).map


#------------------------------------------------------------------------------
# flags
#

# commmon flags propagated to CFLAGS, CXXFLAGS, ASFLAGS (not LDFLAGS)
FLAGS       += $(INCLUDE)
FLAGS       += -Wa,-gdwarf-2
FLAGS       += -MMD

CXXFLAGS     = $(FLAGS)
CXXFLAGS    += $(OPTIMIZER)
CXXFLAGS    += -std=c++11
CXXFLAGS    += -g
CXXFLAGS    += -fno-exceptions
CXXFLAGS    += -fno-rtti

CXXFLAGS    += -Wall -Wextra
CXXFLAGS    += -Winline   # see non-inlined functions
#CXXFLAGS    += -Weffc++
CXXFLAGS    += -Wctor-dtor-privacy
CXXFLAGS    += -Wcast-align -Wpointer-arith -Wredundant-decls
CXXFLAGS    += -Wshadow -Wcast-qual -Wcast-align -pedantic

LD_FLAGS     = $(CPUFLAGS)
#LD_FLAGS    += -Wl,-Map="$(MAP)",--cref
LD_FLAGS    += -fno-exceptions
LD_FLAGS    += -fno-rtti
#! LD_FLAGS += -frepo     # Enable automatic template instantiation at link time
LD_FLAGS    += -Wl,--gc-sections


.PHONY: all clean

all: $(EXE)


$(EXE): $(OBJS)
	$(LD) $(OBJS) $(LD_FLAGS) -o $(EXE)
	$(SIZE) $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) $(EXE)


-include $(DEPENDS)
