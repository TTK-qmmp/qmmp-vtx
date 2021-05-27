include(../../plugins.pri)
include($$PWD/common/common.pri)

TARGET = $$PLUGINS_PREFIX/Input/vtx

HEADERS += decodervtxfactory.h \
           vtxmetadatamodel.h \
           decoder_vtx.h \
           vtxhelper.h
    
SOURCES += decodervtxfactory.cpp \
           vtxmetadatamodel.cpp \
           decoder_vtx.cpp \
           vtxhelper.cpp \
           libvtx/ay8912.c \
           libvtx/lh5dec.c \
           libvtx/vtxfile.c

win32 {
HEADERS += libvtx/mman.h
SOURCES += libvtx/mman.c
}

INCLUDEPATH += $$PWD/libvtx

unix {
    target.path = $$PLUGIN_DIR/Input
    INSTALLS += target
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libvtx.so
}
