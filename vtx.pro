
include($$PWD/common/common.pri)

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

CONFIG += warn_on plugin link_pkgconfig c++11

TEMPLATE = lib

QMAKE_CLEAN += lib$${TARGET}.so

unix {
  CONFIG += link_pkgconfig
  PKGCONFIG += qmmp

  QMMP_PREFIX = $$system(pkg-config qmmp --variable=prefix)
  PLUGIN_DIR = $$system(pkg-config qmmp --variable=plugindir)/Input
  LOCAL_INCLUDES = $${QMMP_PREFIX}/include
  LOCAL_INCLUDES -= $$QMAKE_DEFAULT_INCDIRS
  INCLUDEPATH += $$LOCAL_INCLUDES

  plugin.path = $${PLUGIN_DIR}
  plugin.files = lib$${TARGET}.so
  INSTALLS += plugin
}
