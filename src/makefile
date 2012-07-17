# Main makefile for this project
# By default, build in the 'release' mode. To use debugging, build using:
# make mode=debug all
ifeq ($(mode),debug)
# Debugging flags and objects
   CXXFLAGS = -O0 -g -DDEBUG -DERROR_OUT
   LDFLAGS = -O0 -g
   DEBUG_OBJ_32 = dumpjoystick.o32
   DEBUG_OBJ_64 = dumpjoystick.o64
else
# Release flags and extra debugging objects
   mode = release
   CXXFLAGS = -O2 -DNDEBUG
   LDFLAGS = -O2
   DEBUG_OBJ_32 = 
   DEBUG_OBJ_64 = 
endif

# Common compiling and linking flags
CXXFLAGS += -W -Wall -std=c++98 -pedantic -Wextra -Wconversion -ansi -pedantic -mmacosx-version-min=10.5
LDFLAGS  += -Wl -framework IOKit -framework CoreFoundation -fexception 

# Put your Matlab installation here. Note that if you are only compiling for only one of
# either 32 and 64 bit, use 'make mode=release 64'  (or replace 64 with 32).
MATLAB64    = /Applications/MATLAB_R2011a.app
MATLAB32    = /Applications/MATLAB_R2009a.app

# Use the XCode g++ compiler
CXX         = g++
ARCH32      = -arch i386
ARCH64      = -arch x86_64

# MEX (and s-function) file specific compiler flags
CXXM64FLAGS = -I$(MATLAB64)/extern/include -I$(MATLAB64)/simulink/include -DMATLAB_MEX_FILE
CXXM32FLAGS = -I$(MATLAB32)/extern/include -I$(MATLAB32)/simulink/include -DMATLAB_MEX_FILE

# MEX (and s-function) file specific linking flags
LM64FLAGS = -Wl,-twolevel_namespace -undefined error -bundle -Wl,-exported_symbols_list,$(MATLAB64)/extern/lib/maci64/mexFunction.map -L$(MATLAB64)/bin/maci64 -lmx -lmex -lmat -lstdc++
LM32FLAGS = -Wl,-twolevel_namespace -undefined error -bundle -Wl,-exported_symbols_list,$(MATLAB32)/extern/lib/maci/mexFunction.map -L$(MATLAB32)/bin/maci -lmx -lmex -lmat -lstdc++

# Default target, build all
all: 64 32 test

# 64-bit only target
64: information osx_joystick_get_available.mexmaci64 osx_joystick_get_capabilities.mexmaci64 sfun_osx_joystick.mexmaci64
	@echo "Building the Intel 64-bit Matlab binaries (*.mexmaci64)."

# 32-bit only target
32: information osx_joystick_get_available.mexmaci  osx_joystick_get_capabilities.mexmaci  sfun_osx_joystick.mexmaci
	@echo "Building the Intel 32-bit Matlab binaries (*.mexmaci)"

# Information about the build mode
information: 
ifneq ($(mode),release)
ifneq ($(mode),debug)
	@echo "Build mode '"$(mode)"' is invalid. Please use" 
	@echo "'make mode=release' or 'make mode=debug'"
	@exit 1
endif
endif
	@echo "Building using the '"$(mode)"' mode"

# ALL THE THINGS!
sfun_osx_joystick.mexmaci64: sfun_osx_joystick.o64 osx_joystick.o64 button.o64 axes.o64 pov.o64 outputs.o64 $(DEBUG_OBJ_64)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

sfun_osx_joystick.mexmaci: sfun_osx_joystick.o32 osx_joystick.o32 button.o32 axes.o32 pov.o32 outputs.o32 $(DEBUG_OBJ_32)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)	

sfun_osx_joystick.o64: sfun_osx_joystick.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<
	
sfun_osx_joystick.o32: sfun_osx_joystick.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<

osx_joystick_get_capabilities.mexmaci: osx_joystick_get_capabilities.o32 osx_joystick.o32 button.o32 axes.o32 pov.o32 outputs.o32 $(DEBUG_OBJ_32)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)

osx_joystick_get_capabilities.mexmaci64: osx_joystick_get_capabilities.o64 osx_joystick.o64 button.o64 axes.o64 pov.o64 outputs.o64 $(DEBUG_OBJ_64)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

osx_joystick_get_capabilities.o32: osx_joystick_get_capabilities.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<	

osx_joystick_get_capabilities.o64: osx_joystick_get_capabilities.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<

osx_joystick_get_available.mexmaci: osx_joystick_get_available.o32 osx_joystick.o32 button.o32 axes.o32 pov.o32 outputs.o32 $(DEBUG_OBJ_32)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)

osx_joystick_get_available.mexmaci64: osx_joystick_get_available.o64 osx_joystick.o64 button.o64 axes.o64 pov.o64 outputs.o64 $(DEBUG_OBJ_64)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

osx_joystick_get_available.o32: osx_joystick_get_available.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<	

osx_joystick_get_available.o64: osx_joystick_get_available.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<

test: osx_joystick.o64 test.o button.o64 axes.o64 pov.o64 outputs.o64 $(DEBUG_OBJ_64)
	$(CXX) -o $@ $^ $(LDFLAGS)

test.o: test.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

osx_joystick.o64: osx_joystick.cpp osx_joystick.hpp button.hpp axes.hpp dumpjoystick.hpp pov.hpp outputs.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) -Wno-variadic-macros $<

osx_joystick.o32: osx_joystick.cpp osx_joystick.hpp button.hpp axes.hpp dumpjoystick.hpp pov.hpp outputs.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) -Wno-variadic-macros $<

button.o32: button.cpp button.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
button.o64: button.cpp button.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<
	
axes.o32: axes.cpp axes.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
axes.o64: axes.cpp axes.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<
	
dumpjoystick.o32: dumpjoystick.cpp dumpjoystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
dumpjoystick.o64: dumpjoystick.cpp dumpjoystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<

pov.o32: pov.cpp pov.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
pov.o64: pov.cpp pov.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<

outputs.o32: outputs.cpp outputs.hpp	
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
outputs.o64: outputs.cpp outputs.hpp	
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<

# Cleanup functions
.PHONY: clean cleanest

clean:
	rm -f *.o *.o32 *.o64

cleanest: clean
	rm -f test *.mexmaci64 *.mexmaci
	rm -f ../bin/*.mexmaci64 ../bin/*.mexmaci