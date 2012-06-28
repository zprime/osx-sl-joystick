MATLAB64    = /Applications/MATLAB_R2011a.app
MATLAB32    = /Applications/MATLAB_R2009a.app
CXX         = g++
CXXFLAGS    = -W -Wall -ansi -pedantic -mmacosx-version-min=10.5 -fno-common -no-cpp-precomp -fexceptions
ARCH32      = -arch i386
ARCH64      = -arch x86_64
CXXM64FLAGS = -I$(MATLAB64)/extern/include -I$(MATLAB64)/simulink/include -DMATLAB_MEX_FILE
CXXM32FLAGS = -I$(MATLAB32)/extern/include -I$(MATLAB32)/simulink/include -DMATLAB_MEX_FILE
LDFLAGS   = -framework IOKit -framework CoreFoundation
LM64FLAGS = -O -Wl,-twolevel_namespace -undefined error -bundle -Wl,-exported_symbols_list,$(MATLAB64)/extern/lib/maci64/mexFunction.map -L/$(MATLAB64)/bin/maci64 -lmx -lmex -lmat -lstdc++
LM32FLAGS = -O -Wl,-twolevel_namespace -undefined error -bundle -Wl,-exported_symbols_list,$(MATLAB32)/extern/lib/maci/mexFunction.map -L/$(MATLAB32)/bin/maci -lmx -lmex -lmat -lstdc++

all: JoystickGetAvailable.mexmaci64 JoystickGetAvailable.mexmaci test

JoystickGetAvailable.mexmaci: JoystickGetAvailable.o32 osx_joystick.o32 button.o32
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)

JoystickGetAvailable.mexmaci64: JoystickGetAvailable.o64 osx_joystick.o64 button.o64
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

JoystickGetAvailable.o32: JoystickGetAvailable.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<	

JoystickGetAvailable.o64: JoystickGetAvailable.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<

test: osx_joystick.o64 test.o button.o64 axes.o64
	$(CXX) -o $@ $^ $(LDFLAGS)

test.o: test.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

osx_joystick.o64: osx_joystick.cpp osx_joystick.hpp button.hpp axes.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<

osx_joystick.o32: osx_joystick.cpp osx_joystick.hpp button.hpp axes.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<

button.o32: button.cpp button.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
button.o64: button.cpp button.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<
	
axes.o32: axes.cpp axes.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH32) $<
	
axes.o64: axes.cpp axes.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(ARCH64) $<
.PHONY: clean cleanest

clean:
	rm *.o *.o32 *.o64

cleanest: clean
	rm test JoystickGetAvailable.mexmaci64 JoystickGetAvailable.mexmaci