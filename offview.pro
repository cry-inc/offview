# qmake file for OffView

contains(QT_VERSION, ^4\\.[0-9]\\..*) {
	message("Cannot build OffView with Qt version $${QT_VERSION}.")
	error("Use at least Qt 5.0")
}

QT += opengl

CONFIG += c++11
CONFIG += warn_on
CONFIG += qt

msvc:LIBS += opengl32.lib
mingw:LIBS += -lopengl32

FORMS += src/MainWindow.ui

SOURCES += src/main.cpp \
	src/MainWindow.cpp \
	src/GlWidget.cpp \
	src/CVertex.cpp \
	src/CPolygon.cpp \
	src/OffScene.cpp \
	src/WireframeMode.cpp \
	src/DotMode.cpp \
	src/ShadedMode.cpp \
	src/FlatShadedMode.cpp \
	src/SmoothShadedMode.cpp \
	src/ColoredMode.cpp \
	src/SceneFactory.cpp
    
HEADERS += src/MainWindow.h \
	src/GlWidget.h \
	src/CVertex.h \
	src/CPolygon.h \
	src/IScene.h \
	src/OffScene.h \
	src/IRenderMode.h \
	src/WireframeMode.h \
	src/DotMode.h \
	src/ShadedMode.h \
	src/FlatShadedMode.h \
	src/SmoothShadedMode.h \
	src/ColoredMode.h \
	src/SceneFactory.h
    
TRANSLATIONS += lang/offview_de.ts \
	lang/offview_en.ts

# This file includes the Windows icon
RC_FILE = offview.rc

RESOURCES = offview.qrc

# This file is the Mac OS X icon
ICON = img/offview.icns
