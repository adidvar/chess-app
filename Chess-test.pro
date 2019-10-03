TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += -O3

DISTFILES += \
    .gitignore \
    README.md

SOURCES += \
    Chess.cpp \
    ChessController.cpp \
    Chessboard.cpp \
    MyBot.cpp \
    UI.cpp \
    main.cpp

HEADERS += \
    include/AbstractBot.h \
    include/Chess.h \
    include/ChessController.h \
    include/Chessboard.h \
    include/MyBot.h \
    include/UI.h

LIBS += -LD:/Developing/SFML32/lib/ -lsfml-window -lsfml-graphics -lsfml-system

INCLUDEPATH += D:/Developing/SFML32/include
DEPENDPATH += D:/Developing/SFML32/include
INCLUDEPATH += include
DEPENDPATH += include

