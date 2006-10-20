include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AffineTransform.cpp
HEADERS+=Gear_AffineTransform.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AffineTransform

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AffineTransform
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


