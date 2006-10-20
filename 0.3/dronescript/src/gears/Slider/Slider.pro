include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Slider.cpp GearGui_Slider.cpp ControlSlider.cpp
HEADERS+=Gear_Slider.h GearGui_Slider.h ControlSlider.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Slider

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Slider
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


