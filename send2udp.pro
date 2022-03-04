QT       += core gui network

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/send2udpBin/debug
}else{
    DESTDIR = $$PWD/send2udpBin/release
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customhexeditor.cpp \
    customplaintextedit.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    customhexeditor.h \
    customplaintextedit.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


MOC_DIR = tmp/moc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc
