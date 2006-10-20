include (../../../config.pro)

CONFIG += thread

TEMPLATE=lib
SOURCES+=Gear_AudioInput.cpp
HEADERS+=Gear_AudioInput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/portaudio/include
QMAKE_RPATH = -Wl,-rpath,lib/portaudio,-rpath,

#linux
unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/portaudio -ldroneCore -lportaudio
unix:!macx:TARGET=../../../gears/Gear_AudioInput

#osx
#todo add portaudio
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AudioInput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


