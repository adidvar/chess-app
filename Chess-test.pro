TEMPLATE = app
CONFIG += console c++17 -O3
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    sourse/ABot.h \
    sourse/AObs.h \
    sourse/APlayer.h \
    sourse/Chess.h \
    sourse/Chessboard.h \
    sourse/ChessController.h \
    sourse/MyBot.h \
    sourse/UI.h \
    sourse/NetPlayer.h

SOURCES += \
    sourse/Chess.cpp \
    sourse/Chessboard.cpp \
    sourse/ChessController.cpp \
    sourse/main.cpp \
    sourse/MyBot.cpp \
    sourse/UI.cpp \
    sourse/NetPlayer.cpp

LIBS += -L$$PWD/../SFML/lib/ -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lpthread

INCLUDEPATH += $$PWD/../SFML/include
DEPENDPATH += $$PWD/../SFML/include
