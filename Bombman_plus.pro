QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    beginscene.cpp \
    bomb.cpp \
    component.cpp \
    gameobject.cpp \
    gamescene.cpp \
    givememoney.cpp \
    grounditem.cpp \
    imagetransform.cpp \
    introduction.cpp \
    main.cpp \
    mainwindow.cpp \
    mapoperator.cpp \
    mypushbutton.cpp \
    physics.cpp \
    player.cpp \
    props.cpp \
    robot.cpp \
    transform.cpp \
    transformbuilder.cpp

HEADERS += \
    beginscene.h \
    bomb.h \
    component.h \
    gameobject.h \
    gamescene.h \
    givememoney.h \
    grounditem.h \
    imagetransform.h \
    introduction.h \
    mainwindow.h \
    mapoperator.h \
    mypushbutton.h \
    physics.h \
    player.h \
    props.h \
    robot.h \
    transform.h \
    transformbuilder.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
