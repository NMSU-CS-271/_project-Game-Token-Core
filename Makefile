CC = g++
CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -lfltk -lfltk_images -lX11 -lpthread
VPATH = GUI

game_loop: Complex_circle.o Window_game.o Driver_scripted.o Game_state.o Board.o Board_space.o Graph.o GUI.o Window.o

game_loop_headless: Driver_scripted.o Game_state.o Board.o Board_space.o

clean:
	rm -f *.o
	rm -f game_loop
	rm -f game_loop_headless