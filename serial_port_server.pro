#-------------------------------------------------
#
# Project created by QtCreator 2024-06-15T01:33:17
#
#-------------------------------------------------

QT       += core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serial_port_server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        datamanager.cpp \
        homepage.cpp \
        loginmanager.cpp \
        logsmanager.cpp \
        main.cpp \
        mytcpclient.cpp \
        myudp.cpp \
        tcpclient.cpp \
        tcpserver.cpp \
        uartmanager.cpp \
        usersmanager.cpp \
        widget.cpp \
        workmodemanager.cpp

HEADERS += \
        datamanager.h \
        homepage.h \
        loginmanager.h \
        logsmanager.h \
        mytcpclient.h \
        myudp.h \
        tcpclient.h \
        tcpserver.h \
        uartmanager.h \
        usersmanager.h \
        widget.h \
        workmodemanager.h

FORMS += \
        homepage.ui \
        loginmanager.ui \
        logsmanager.ui \
        uartmanager.ui \
        usersmanager.ui \
        widget.ui \
        workmodemanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc \
    res.qrc
