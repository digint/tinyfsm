# Compiler prefix, in case your default compiler does not implement all c++11 features:
#CROSS = /opt/toolchain/x86_64-pc-linux-gnu-gcc-4.7.0/bin/x86_64-pc-linux-gnu-

# HINT: g++ -Q -O2 --help=optimizers
OPTIMIZER    = -Os

CC           = $(CROSS)gcc
CXX          = $(CROSS)g++
#SIZE         = $(CROSS)size -B -d
SIZE         = size -B -d
RM           = rm -f

SRC_DIRS     = .
INCLUDE      = -I ../../include

SRCS         = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS         = $(SRCS:.cpp=.o)
DEPENDS      = $(OBJS:.o=.d)

EXE          = $(SRCS:.cpp=)


#------------------------------------------------------------------------------
# flags
#

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


.PHONY: all clean

all: $(EXE)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	$(SIZE) $@

clean:
	$(RM) *.d
	$(RM) $(EXE)


-include $(DEPENDS)
