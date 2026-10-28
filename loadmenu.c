#include "Ass1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

char *options[] = {
			"How To Play",
			"Load Save File",
			"New Game",
			"Exit",
		  };

void print_rules();

int main() {
	ITEM **menu_options;
	int ch;
	bool enter_pressed = false;
	MENU *game_menu;
	int n_options, i;

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	
	n_options = ARRAY_SIZE(options);
	menu_options = (ITEM **)calloc(n_options + 1, sizeof(ITEM *));
	for (i = 0; i < n_options; ++i)
		menu_options[i] = new_item(options[i], "");
	menu_options[n_options] = (ITEM *)NULL;

	game_menu = new_menu((ITEM **)menu_options);
	post_menu(game_menu);
	refresh();

	while (1) {
		enter_pressed = false;
		ch = getch();
		//switch for handling menu movement and what happens when enter is pressed
		switch (ch) {
			case KEY_DOWN:
				menu_driver(game_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(game_menu, REQ_UP_ITEM);
				break;
			case 10:
				enter_pressed = true;
				break;	
		}
		if (enter_pressed) {
			//clear previous text below menu
			move(5, 0);
			clrtobot();
			refresh();
			//if player selects "how to play"
			//print rules under menu
			if (current_item(game_menu) == menu_options[0]) {
				print_rules();
				continue;
			}

			//if player selects "Load Save File"
			//check if save file exists
			//if so, call play_game function with 1 as arg
			//to signify a load from save
			if (current_item(game_menu) == menu_options[1]) {
				if (save_exists()) {
					clear();
					play_game(1);
					break;
				}

				else {
					mvprintw(5, 0, "Save file does not exist");
					continue;
				}
			}

			//if player selects "New Game"
			//call play_game function with arg 0
			//to run game code without save data
			if (current_item(game_menu) == menu_options[2]) {
				clear();
				play_game(0);
				break;
			}

			//if player selects "Exit"
			//simply break from the loop
			//exit the program
			if (current_item(game_menu) == menu_options[3]) {
				break;
			}
		}
	}

	//freeing menu data
	unpost_menu(game_menu);
	free_menu(game_menu);
	for (i = 0; i < n_options; ++i)
		free_item(menu_options[i]);
	//exit from curses mode
	endwin();
	
	return 0;
}

//function for printing rules of the game under the menu
void print_rules() {
	mvprintw(5, 0, "Rules of the 5x5 grid game:\n"
			"You must fill the 5x5 grid with the numbers 1-25 in an incremental manner\n"
			"(First place 1, then 2, and so on...)\n\n"
			"Use the arrow keys to navigate the grid\n"
			"Press enter to place a number\n\n"
			"Numbers must be placed adjacent to the previous number\n"
			"(up, down, left, right, or diagonal)\n"
			"Numbers cannot be placed on top of each other\n\n"
			"1 point is earned when a number is placed diagonal to the previous number\n"
			"0 points are earned otherwise"); 
	return;
}
