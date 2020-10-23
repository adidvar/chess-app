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
#    turngenerator_bench.cpp

HEADERS += \
    board.h \
    bot.h \
    map_preview.h \
    player.h \
    turn.h \
    turnexecutor.h \
    turngenerator.h

LIBS += -LD:/Libs/SFMLx32/lib/ -lsfml-system -lsfml-window -lsfml-graphics

INCLUDEPATH += D:/Libs/SFMLx32/include
DEPENDPATH += D:/Libs/SFMLx32/include
