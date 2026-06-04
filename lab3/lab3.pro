QT += core gui widgets network

CONFIG += c++17

TARGET = lab3
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    httpclient.cpp \
    raceresult.cpp

HEADERS += \
    mainwindow.h \
    httpclient.h \
    raceresult.h
