#-------------------------------------------------
#
# Project created by QtCreator 2017-08-31T10:44:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network axcontainer

TARGET = ERC69Accounting
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        startscreen.cpp \
    contacts.cpp \
    financialmutations.cpp \
    jsonintrepeter.cpp \
    playercard.cpp \
    salesinvoice.cpp \
    processplayercards.cpp \
    processfinancialmutations.cpp

HEADERS += \
        startscreen.h \
    contacts.h \
    financialmutations.h \
    jsonintrepeter.h \
    playercard.h \
    salesinvoice.h \
    processplayercards.h \
    processfinancialmutations.h

FORMS += \
        startscreen.ui \
    processplayercards.ui \
    processfinancialmutations.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MoneybirdAPI-Desktop_Qt_5_9_1_MinGW_32bit-Debug/release/ -lMoneybirdAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MoneybirdAPI-Desktop_Qt_5_9_1_MinGW_32bit-Debug/debug/ -lMoneybirdAPI

INCLUDEPATH += $$PWD/../MoneybirdAPI
DEPENDPATH += $$PWD/../MoneybirdAPI

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-MoneybirdAPI-Desktop_Qt_5_9_1_MinGW_32bit-Debug/release/libMoneybirdAPI.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-MoneybirdAPI-Desktop_Qt_5_9_1_MinGW_32bit-Debug/debug/libMoneybirdAPI.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-MoneybirdAPI-Desktop_Qt_5_9_1_MinGW_32bit-Debug/release/MoneybirdAPI.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-MoneybirdAPI-Desktop_Qt_5_9_1_MinGW_32bit-Debug/debug/MoneybirdAPI.lib
