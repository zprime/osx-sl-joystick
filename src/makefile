MATLAB64    = /Applications/MATLAB_R2011a.app
MATLAB32    = /Applications/MATLAB_R2009a.app
CXX         = g++
CXXFLAGS    = -O2 -W -Wall -ansi -pedantic -mmacosx-version-min=10.5
ARCH32      = -arch i386
ARCH64      = -arch x86_64
CXXM64FLAGS = -I$(MATLAB64)/extern/include -I$(MATLAB64)/simulink/include -DMATLAB_MEX_FILE
CXXM32FLAGS = -I$(MATLAB32)/extern/include -I$(MATLAB32)/simulink/include -DMATLAB_MEX_FILE
LDFLAGS   = -framework IOKit -framework CoreFoundation -fexception
LM64FLAGS = -O -Wl,-twolevel_namespace -undefined error -bundle -Wl,-exported_symbols_list,$(MATLAB64)/extern/lib/maci64/mexFunction.map -L$(MATLAB64)/bin/maci64 -lmx -lmex -lmat -lstdc++
LM32FLAGS = -O -Wl,-twolevel_namespace -undefined error -bundle -Wl,-exported_symbols_list,$(MATLAB32)/extern/lib/maci/mexFunction.map -L$(MATLAB32)/bin/maci -lmx -lmex -lmat -lstdc++

all: osx_joystick_get_available.mexmaci64 osx_joystick_get_available.mexmaci osx_joystick_get_capabilities.mexmaci64 osx_joystick_get_capabilities.mexmaci sfun_osx_joystick.mexmaci64 sfun_osx_joystick.mexmaci test

sfun_osx_joystick.mexmaci64: sfun_osx_joystick.o64 osx_joystick.o64 button.o64 axes.o64 pov.o64 dumpjoystick.o64 outputs.o64
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

sfun_osx_joystick.mexmaci: sfun_osx_joystick.o32 osx_joystick.o32 button.o32 axes.o32 pov.o32 dumpjoystick.o32 outputs.o32
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)	

sfun_osx_joystick.o64: sfun_osx_joystick.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<
	
sfun_osx_joystick.o32: sfun_osx_joystick.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<

osx_joystick_get_capabilities.mexmaci: osx_joystick_get_capabilities.o32 osx_joystick.o32 button.o32 axes.o32 pov.o32 dumpjoystick.o32 outputs.o32
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)

osx_joystick_get_capabilities.mexmaci64: osx_joystick_get_capabilities.o64 osx_joystick.o64 button.o64 axes.o64 pov.o64 dumpjoystick.o64 outputs.o64
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

osx_joystick_get_capabilities.o32: osx_joystick_get_capabilities.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<	

osx_joystick_get_capabilities.o64: osx_joystick_get_capabilities.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<

osx_joystick_get_available.mexmaci: osx_joystick_get_available.o32 osx_joystick.o32 button.o32 axes.o32 pov.o32 dumpjoystick.o32 outputs.o32
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM32FLAGS) $(ARCH32)

osx_joystick_get_available.mexmaci64: osx_joystick_get_available.o64 osx_joystick.o64 button.o64 axes.o64 pov.o64 dumpjoystick.o64 outputs.o64
	$(CXX) -o $@ $^ $(LDFLAGS) $(LM64FLAGS) $(ARCH64)

osx_joystick_get_available.o32: osx_joystick_get_available.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM32FLAGS) $(ARCH32) $<	

osx_joystick_get_available.o64: osx_joystick_get_available.cpp osx_joystick.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXM64FLAGS) $(ARCH64) $<

test: osx_joystick.o64 test.o button.o64 axes.o64 dumpjoystick.o64 pov.o64 outputs.o64
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

.PHONY: clean cleanest

clean:
	rm -f *.o *.o32 *.o64

cleanest: clean
	rm -f test *.mexmaci64 *.mexmaci
	rm -f ../bin/*.mexmaci64 ../bin/*.mexmaci