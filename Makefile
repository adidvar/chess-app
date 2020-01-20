CC=g++
Objects=UI.o main.o Chess.o MyBot.o NetPlayer.o Chessboard.o ChessController.o
Include=-I/usr/include/SFML
Lib=-L/usr/lib/x86_64-linux-gnu -lsfml-window -lsfml-system -lsfml-network -lsfml-graphics -pthread
Flags=-m64 -Ofast -flto -march=native -funroll-loops

all: $(Objects)
	$(CC) $(Objects) -o build/Chess-app  $(Flags) $(Lib)

clear:
	rm $(Objects)

%.o: sourse/%.cpp
	$(CC) -c $< -o $@ $(Flags) $(Include)
