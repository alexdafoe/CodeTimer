#-------------------------------------------------
#
# Project created by QtCreator 2018-10-17T13:21:18
#
#-------------------------------------------------

QT       += core qml quick widgets sql
CONFIG += c++11
TARGET = CodeTimer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been noteed as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
    src/controller.cpp \
    src/keyeventfilter.cpp \
    src/trayiconwidget.cpp \
    src/database/database.cpp \
    src/database/databasemodel.cpp \
    src/database/databasecontroller.cpp \
    src/timer.cpp \
    src/log.cpp \
    src/timerdata.cpp \
    src/symbolssettings.cpp \

HEADERS += \
    include/controller.h \
    include/keyeventfilter.h \
    include/timesectostring.h \
    include/trayiconwidget.h \
    include/database/database.h \
    include/database/databasemodel.h \
    include/database/databasecontroller.h \
    include/timer.h \
    include/log.h \
    include/timerdata.h \
    include/symbolssettings.h \

FORMS +=

RC_FILE += \
        icon.rc

RESOURCES += \
    res.qrc

DEFINES += "SEC_DEVIDER=1000000000"

win32-msvc* {
    LIBS += -luser32
    DEFINES -= "SEC_DEVIDER=1000000000"
    DEFINES += "SEC_DEVIDER=10000000";
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
