TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    include/ABot.h \
    include/AObs.h \
    include/APlayer.h \
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

SOURCES += \
    sourse/bots/MyBot.cpp \
    sourse/bots/UI.cpp \
    sourse/core/Chess.cpp \
    sourse/core/ChessController.cpp \
    sourse/core/Chessboard.cpp \
    sourse/main.cpp

