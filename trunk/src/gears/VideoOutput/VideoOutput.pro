include (../config.pro)

TEMPLATE=lib

CONFIG+=thread opengl

#common
SOURCES+=Gear_VideoOutput.cpp VideoOutputMaker.cpp 
HEADERS+=Gear_VideoOutput.h VideoOutput.h VideoOutputMaker.h  

#linux

unix:!macx:SOURCES+=VideoOutputGl.cpp VideoOutputShm.cpp VideoOutputX11Base.cpp VideoOutputXv.cpp VideoOutputQT.cpp
unix:!macx:HEADERS+=VideoOutputGl.h VideoOutputShm.h VideoOutputX11Base.h VideoOutputXv.h VideoOutputQT.h
unix:!macx:INCLUDEPATH+=../../core/ ../../core/types /usr/X11R6/include
unix:!macx:LIBS+= -L../../../lib/ -L/usr/X11R6/lib/ -ldroneCore -lGL -lXv -lXxf86vm
unix:!macx:TARGET=../../../gears/Gear_VideoOutput

#osx

macx:SOURCES+=VideoOutputQT.cpp VideoOutputGl.cpp
macx:HEADERS+=VideoOutputQT.h VideoOutputGl.h

macx:INCLUDEPATH+=../../core/ ../../core/types
macx:INCLUDEPATH+= /Developer/Headers/FlatCarbon
macx:LIBS += /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
macx:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoOutput

