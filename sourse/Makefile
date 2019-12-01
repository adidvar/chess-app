COMP=g++

OUTPUT =..\build\a.exe
INCLUDE =-I"D:\Developing\SFML32\include" -I"."
LIB =-LD:\Developing\SFML32\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lopengl32 -lgdi32 -lsfml-system-s -lwinmm
DEF =-DSFML_STATIC
FLG =-O3 -m32 -mfpmath=sse -Ofast -flto -march=native -static-libgcc -static-libstdc++

.PHONY : all start clear

all : Chess.o Chessboard.o ChessController.o MyBot.o UI.o main.o 
	$(COMP) -o $(OUTPUT) Chess.o main.o Chessboard.o ChessController.o MyBot.o UI.o  $(FLG) $(LIB) $(DEF)
	
start : all
	./$(OUTPUT)

clear :
	rm *.o 
	rm $(OUTPUT)

Chess.o : Chess.cpp Chess.h ABot.h AObs.h APlayer.h
	$(COMP) -c $< -o $@ $(FLG) $(INCLUDE) $(DEF)
Chessboard.o : Chessboard.cpp Chessboard.h Chess.o
	$(COMP) -c $< -o $@ $(FLG) $(INCLUDE) $(DEF)
ChessController.o : ChessController.cpp ChessController.h  Chessboard.o
	$(COMP) -c $< -o $@ $(FLG) $(INCLUDE) $(DEF)
MyBot.o : MyBot.cpp MyBot.h Chessboard.o
	$(COMP) -c $< -o $@ $(FLG) $(INCLUDE) $(DEF)
UI.o : UI.cpp UI.h Chessboard.o
	$(COMP) -c $< -o $@ $(FLG) $(INCLUDE) $(DEF)
main.o : main.cpp Chessboard.o
	$(COMP) -c $< -o $@ $(FLG) $(INCLUDE) $(DEF)