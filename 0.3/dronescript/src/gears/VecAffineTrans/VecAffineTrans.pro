include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VecAffineTrans.cpp \
  ../../../lib/agg23/svg/agg_svg_parser.cpp \
  ../../../lib/agg23/svg/agg_svg_path_renderer.cpp \
  ../../../lib/agg23/svg/agg_svg_path_tokenizer.cpp

HEADERS+=Gear_VecAffineTrans.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/agg23/include ../../../lib/agg23/svg

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/agg23/src/ -lagg
unix:!macx:TARGET=../../../gears/Gear_VecAffineTrans

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VecAffineTrans
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


