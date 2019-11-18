QT += gui widgets multimedia

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_DEBUG_PLUGINS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gpiointerface.cpp \
        main.cpp \
        mediaplayer.cpp \
        playerwidget.cpp \
        rfidinterface.cpp \
        tag.cpp \
        tagmanager.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    gpiointerface.h \
    mediaplayer.h \
    playerwidget.h \
    rfidinterface.h \
    tag.h \
    tagmanager.h \
    MFRC522.cpp

unix:!macx: {
    SOURCES += \

    HEADERS += \
        MFRC522.h \
        bcm2835.h

    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib/ -lbcm2835 -lwiringPi

    CONFIG += link_pkgconfig

    PKGCONFIG += gstreamer-1.0 \
            glib-2.0 \
            gobject-2.0 \
            gio-2.0
}



FORMS += \
    playerwidget.ui
