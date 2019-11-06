cd ..
g++ Chess-test\Chess.cpp Chess-test\Chessboard.cpp Chess-test\ChessController.cpp Chess-test\MyBot.cpp Chess-test\UI.cpp Chess-test\main.cpp -IChess-test\include -ID:\Developing\SFML32\include -c -O3 -DSFML_STATIC -m32 -mfpmath=sse -Ofast -flto -march=native
g++ Chess.o  Chessboard.o ChessController.o MyBot.o UI.o main.o -LD:\Developing\SFML32\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lopengl32 -lgdi32 -lsfml-system-s -lwinmm -static-libgcc -static-libstdc++ -m32 -mfpmath=sse -Ofast -flto -march=native
pause



