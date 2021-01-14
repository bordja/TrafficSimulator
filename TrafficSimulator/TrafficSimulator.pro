#-------------------------------------------------
#  Copyright 2019 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TARGET = TrafficSimulator 
TEMPLATE = app

CONFIG += c++14

# additional modules are pulled in via arcgisruntime.pri
QT += opengl widgets


equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 12) { 
        error("$$TARGET requires Qt 5.12.6")
    }
	equals(QT_MINOR_VERSION, 12) : lessThan(QT_PATCH_VERSION, 6) {
		error("$$TARGET requires Qt 5.12.6")
	}
}

ARCGIS_RUNTIME_VERSION = 100.8
include($$PWD/arcgisruntime.pri)

win32:CONFIG += \
    embed_manifest_exe

DATA_FILES += \
    $$PWD/data

SOURCES += \
    frame.cpp \
    main.cpp \
    TrafficSimulator.cpp  \
    mapobject.cpp \
    stream.cpp \
    streamconstants.cpp \
    streammanager.cpp

HEADERS += \
    TrafficSimulator.h \
    common.h \
    frame.h \
    mapobject.h \
    stream.h \
    streamconstants.h \
    data/out_perspective_1 \
    data/out_perspective_3 \
    streammanager.h

# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

copyToDestDir($$DATA_FILES, $$OUT_PWD/data/)

#-------------------------------------------------------------------------------
