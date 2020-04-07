include(../../plugins.pri)
include($$PWD/common/common.pri)

TARGET = $$PLUGINS_PREFIX/Input/vtx

HEADERS += decodervtxfactory.h \
           vtxmetadatamodel.h \
           decoder_vtx.h \
           vtxhelper.h \
           ayemu.h \
           ayemu_8912.h \
           ayemu_vtxfile.h
    
SOURCES += decodervtxfactory.cpp \
           vtxmetadatamodel.cpp \
           decoder_vtx.cpp \
           vtxhelper.cpp \
           ay8912.c \
           lh5dec.c \
           vtxfile.c

win32 {
HEADERS += mman.h
SOURCES += mman.c
}

unix {
    target.path = $$PLUGIN_DIR/Input
    INSTALLS += target
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libvtx.so
}
