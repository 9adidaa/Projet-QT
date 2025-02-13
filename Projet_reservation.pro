QT += core gui sql widgets printsupport network charts serialport

CONFIG += c++11

SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    reservation.cpp


HEADERS += \
    connection.h \
    mainwindow.h \
    reservation.h


INCLUDEPATH += $$PWD/SmtpClient/src

FORMS += \
    mainwindow.ui
