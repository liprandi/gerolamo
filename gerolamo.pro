QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /opt/vc/include

LIBS += -L/opt/vc/lib -lbcm_host -lwiringPi

SOURCES += \
    main.cpp \
    hmi.cpp \
    piinfo.cpp \
    hx711.cpp \
    rtthread.cpp \
    debugwidget.cpp \
    zcontrol.cpp \
    stepper.cpp

HEADERS += \
    hmi.h \
    piinfo.h \
    hx711.h \
    rtthread.h \
    debugwidget.h \
    zcontrol.h \
    stepper.h

FORMS += \
    hmi.ui \
    debugwidget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc