######################################################################
# Automatically generated by qmake (1.07a) Sat May 22 15:57:15 2004
######################################################################

TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += . src /usr/include/libmpeg3
LIBS += -lsndfile -lfftw3 -lXxf86vm -lmpeg3 -lGLU -lm -lportaudio -lGL -lqt
CONFIG += thread 
# Input
HEADERS += config.h \
           src/Canvas.h \
           src/CircularBufferSignal.h \
           src/ConnectionItem.h \
           src/Engine.h \
           src/FrequencyAnalyzer.h \
           src/Gear.h \
           src/Gear_AudioInput.h \
           src/Gear_AudioOutput.h \
           src/Gear_AudioSource.h \
           src/Gear_AverageSignal.h \
           src/Gear_Blur.h \
           src/Gear_ColorAdjust.h \
           src/Gear_Contrast.h \
       	   src/Gear_ColorQuantize.h \
           src/Gear_ColorQuantizeWu.h \
           src/Gear_ClusteredDither.h \
           src/Gear_DataOut.h \
           src/Gear_DiffDist.h \
           src/Gear_Edge.h \
           src/Gear_FilterSignal.h \
           src/Gear_FreqBoxes.h \
           src/Gear_Gain.h \
           src/Gear_GrayScale.h \
           src/Gear_HalfToning.h \
           src/Gear_Image.h \
           src/Gear_KDTree.h \
           src/Gear_MasterFrequency.h \
           src/Gear_MedianFilter.h \
           src/Gear_NGone.h \
           src/Gear_Osc.h \
           src/Gear_Oscilloscope.h \
           src/Gear_PatternGen.h \
           src/Gear_Saturation.h \
           src/Gear_SimpleDelay.h \
           src/Gear_Slider.h \
           src/Gear_TV.h \
           src/Gear_VideoAdd.h \
           src/Gear_VideoBlend.h \
           src/Gear_VideoInput.h \
           src/Gear_VideoOutput.h \
           src/Gear_VideoSource.h \
           src/GearGui.h \
           src/GearGui_Slider.h \
           src/GearGui_TV.h \
           src/GearMaker.h \
           src/GearPropertiesDialog.h \
           src/MainWindow.h \
           src/Plug.h \
           src/PlugBox.h \
           src/PlugSignal.h \
           src/PlugSignalIn.h \
           src/PlugSignalOut.h \
           src/PlugVideo.h \
           src/PlugVideoCompose.h \
           src/PlugVideoComposeIn.h \
           src/PlugVideoComposeOut.h \
           src/PlugVideoIn.h \
           src/PlugVideoOut.h \
           src/Properties.h \
           src/PropertyControl.h \
           src/PropertyControlFilename.h \
           src/PropertyControlString.h \
           src/Rasterer.h \
           src/SummedAreaTable.h \
           src/SchemaEditor.h \
           src/Signal.h \
           src/SignalCircularBuffer.h \
           src/SignalInfo.h \
           src/Timing.h \
           src/Typedefs.h \
           src/VideoInfo.h \
	   src/Play.xpm \
	   src/Pause.xpm \
	   src/splash.xpm \
	   COPYING	   
SOURCES += src/Canvas.cpp \
           src/CircularBufferSignal.cpp \
           src/ConnectionItem.cpp \
           src/drone.cpp \
           src/Engine.cpp \
           src/FrequencyAnalyzer.cpp \
           src/Gear.cpp \
           src/Gear_AudioInput.cpp \
           src/Gear_AudioOutput.cpp \
           src/Gear_AudioSource.cpp \
           src/Gear_AverageSignal.cpp \
           src/Gear_Blur.cpp \
           src/Gear_ColorAdjust.cpp \
           src/Gear_Contrast.cpp \
           src/Gear_ColorQuantize.cpp \
           src/Gear_ColorQuantizeWu.cpp \
           src/Gear_ClusteredDither.cpp \
           src/Gear_DataOut.cpp \
           src/Gear_DiffDist.cpp \
           src/Gear_Edge.cpp \
           src/Gear_FilterSignal.cpp \
           src/Gear_FreqBoxes.cpp \
           src/Gear_Gain.cpp \
           src/Gear_GrayScale.cpp \
           src/Gear_HalfToning.cpp \
           src/Gear_Image.cpp \
           src/Gear_KDTree.cpp \
           src/Gear_MasterFrequency.cpp \
           src/Gear_MedianFilter.cpp \
           src/Gear_NGone.cpp \
           src/Gear_Osc.cpp \
           src/Gear_Oscilloscope.cpp \
           src/Gear_PatternGen.cpp \
           src/Gear_Saturation.cpp \
           src/Gear_SimpleDelay.cpp \
           src/Gear_Slider.cpp \
           src/Gear_TV.cpp \
           src/Gear_VideoAdd.cpp \
           src/Gear_VideoBlend.cpp \
           src/Gear_VideoInput.cpp \
           src/Gear_VideoOutput.cpp \
           src/Gear_VideoSource.cpp \
           src/GearGui.cpp \
           src/GearGui_Slider.cpp \
           src/GearGui_TV.cpp \
           src/GearMaker.cpp \
           src/GearPropertiesDialog.cpp \
           src/MainWindow.cpp \
           src/Plug.cpp \
           src/PlugBox.cpp \
           src/PlugSignalIn.cpp \
           src/PlugSignalOut.cpp \
           src/PlugVideoIn.cpp \
           src/PlugVideoOut.cpp \
           src/Properties.cpp \
           src/PropertyControl.cpp \
           src/PropertyControlFilename.cpp \
           src/PropertyControlString.cpp \
           src/Rasterer.cpp \
           src/SchemaEditor.cpp \
           src/SignalCircularBuffer.cpp \
           src/SignalInfo.cpp \
           src/SummedAreaTable.cpp \
           src/Timing.cpp \
           src/VideoInfo.cpp
	   
