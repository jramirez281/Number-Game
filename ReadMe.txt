Josueh Ramirez
CEN4020
Dr. Suey-Chyun Fang

Description:
	- There are four source code files, three with c code
	and one header file where functions are declared

	- The source code is written in C, and makes heavy use 
	of the ncurses library and its menu extension.

	- loadmenu.c contains the main function, it is
	where the other functions are loaded from. It includes
	the code for displaying the menu items and the 
	handling for each selection.

	- grid.c contains the main game code. It includes the 
	code for displaying the 5x5 grid, as well as the 
	logic for earning points and stopping the game
	when an illegal move has been made. 

	- Additionally, grid.c contains logic for handling 
	loading a game from a save file, and the function
	play_game() has a parameter for determining
	if a new game should be played or if a previous game is
	being continued.

	- save.c contains the code for saving an incomplete 
	game to a text file save_file.txt. This text file stores 
	important variables from the game in a specific order. 
	This order allows the game to easily recover its 
	save state.

OS: Debian Linux 13
Compiler: gcc 14.2.0
Compilation Command: gcc -o Ass1 loadmenu.c grid.c save.c -lncurses -lmenu -Wall

 
