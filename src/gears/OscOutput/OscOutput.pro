include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscOutput.cpp
HEADERS+=Gear_OscOutput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/liblo/src/.libs -llo
unix:!macx:TARGET=../../../gears/Gear_OscOutput

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_OscOutput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -llo


