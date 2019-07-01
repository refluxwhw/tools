TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/bin

SOURCES += \
        main.c

win32 {
include(win/win.pri)
}
