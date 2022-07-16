
QT += multimedia

HEADERS += \
    $$PWD/audioread.h \
    $$PWD/audiowrite.h \
    $$PWD/world.h \
    $$PWD/audioreadqdialog.h

SOURCES += \
    $$PWD/audioread.cpp \
    $$PWD/audiowrite.cpp \
    $$PWD/audioreadqdialog.cpp


LIBS +=$$PWD/speex/lib/libspeex.lib

FORMS += \
    $$PWD/audioreadqdialog.ui
