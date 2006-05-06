######################################################################
# Automatically generated by qmake (1.07a) Sat May 22 15:57:15 2004
######################################################################

CONFIG = qt warn_on thread 
include (../../config.pro)

message(Drone : Builing Makefile with the following config flags : $$CONFIG)


unix:!macx:QMAKE_CXXFLAGS += -rdynamic

TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += ../core ../core/types ../contrib/frei0r/spec/

#mac
macx:LIBS += -L../../drone.app/Contents/Frameworks -ldroneCore
macx:LIBS += /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
macx:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
RC_FILE = myapp.icns
QMAKE_INFO_PLIST = Info.plist
#bundling on osx
macx:bundle.target = bundle
macx:bundle.commands += ./bundle.osx
macx:PRE_TARGETDEPS += bundle
macx:QMAKE_EXTRA_UNIX_TARGETS += bundle

#linux
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/,-rpath,
unix:!macx:LIBS += -L../../lib -ldroneCore

TAERGET=drone
DESTIR = ../../

HEADERS += config.h \
GearListMenu.h \
GearNavigatorView.h \
GearListView.h \
HelpWindow.h \
HelpBrowser.h \
HelpEditor.h \
MediaPoolIconView.h \
MediaPoolItem.h \
MainWindow.h \
MetaGearEditor.h \
MetaGearListMenu.h \
MediaPoolDialog.h \
#PreferencesDialog.h \
PlugsTable.h \
PlugsTableItem.h \
PlugsTableItemFilename.h \
PlugsTableItemValue.h \
PlugsTableItemString.h \
SchemaEditor.h \
PaletteWidget.h \
PaletteManager.h \
ToolTitleBar.h \
SelectFilenameWidget.h \
Play.xpm \
Pause.xpm \
splash.xpm

SOURCES += drone.cpp \
GearListMenu.cpp \
GearListView.cpp \ 
GearNavigatorView.cpp \
HelpBrowser.cpp \
HelpWindow.cpp \
HelpEditor.cpp \
MainWindow.cpp \
MediaPoolIconView.cpp \
MediaPoolItem.cpp \
MetaGearEditor.cpp \
MetaGearListMenu.cpp \
MediaPoolDialog.cpp \
PaletteManager.cpp \
PaletteWidget.cpp \
PlugsTable.cpp \
PlugsTableItem.cpp \
PlugsTableItemFilename.cpp \
PlugsTableItemValue.cpp \
PlugsTableItemString.cpp \
#PreferencesDialog.cpp \
SchemaEditor.cpp \
SelectFilenameWidget.cpp \
ToolTitleBar.cpp
