include(config.pro)

TEMPLATE=subdirs
SUBDIRS= Osc \
TV \
Oscilloscope \
VideoOutput \
Blur \
VideoSource \
Slider \
KDTree\ 
VideoMix \
Edge \
AffineTransform \
ApplyDisplaceGrid \
AreaArrayMask \
#BeatDetector \
Brightness \
ClusteredDither \
ColorAdjust \
Contrast \
Enveloppe \
FlatSignal \
ListBox \
PushButton \
Rescale \
TouchPad \
ColorQuantize \
Gamma \
GenDisplaceGrid \
GrayScale \
HalfToning \
Saturation \
SignalStat \
SmearGrid \
Sobel \
#VideoDelay \
#VideoLoop \
ImageShuffle \
ImageSequence \
Invert
#VideoRecord 

agg {
SUBDIRS+=AggTest
}

#linux specific gears
unix:!macx:SUBDIRS += VideoInput AudioInput

