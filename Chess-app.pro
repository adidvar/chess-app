TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    sourse/UI.cpp \
    sourse/NetPlayer.cpp \
    sourse/MyBot.cpp \
    sourse/main.cpp \
    sourse/ChessController.cpp \
    sourse/Chessboard.cpp \
    sourse/Chess.cpp

HEADERS += \
    sourse/UI.h \
    sourse/NetPlayer.h \
    sourse/MyBot.h \
    sourse/ChessController.h \
    sourse/Chessboard.h \
    sourse/Chess.h \
    sourse/APlayer.h \
    sourse/AObs.h \
    sourse/ABot.h

LIBS += -L/usr/lib/x86_64-linux-gnu/ -lsfml-system -lpthread
LIBS += -lsfml-window
LIBS += -lsfml-network
LIBS += -lsfml-graphics

INCLUDEPATH += /usr/include/SFML
DEPENDPATH += /usr/include/SFML
