TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        board.cpp \
#        board_test.cpp \
    bot.cpp \
     main.cpp \
    map_preview.cpp \
    player.cpp \
        turn.cpp \
    turnexecutor.cpp \
    turngenerator.cpp \
   # turngenerator_bench.cpp

HEADERS += \
    board.h \
    bot.h \
    map_preview.h \
    player.h \
    turn.h \
    turnexecutor.h \
    turngenerator.h

LIBS += -L/usr/lib/x86_64-linux-gnu -lsfml-system -lsfml-window -lsfml-graphics -pthread

INCLUDEPATH += /usr/include/SFML
DEPENDPATH += /usr/include/SFML
