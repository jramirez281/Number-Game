#include "Ass1.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 25
#define HEIGHT 25


//function for printing 5x5 grid with current state
void print_grid(WINDOW *grid_win, int highlighty, int highlightx, int nrows, int ncols, int choices[nrows][ncols]);

//main game engine
//save_data parameter for determining if we are
//starting a new game or loading from save data
void play_game(int save_data) {
	//initializing variables
	int startx = 0;
	int starty = 0;

	int grid_rows = 5;
	int grid_cols = 5;

	//2d array for keeping state of grid persistent
	int choices[grid_rows][grid_cols];
	memset(choices, 0, sizeof choices);
	int i = 0;
	int score = 0;
	int prev_choice[2] = {-1,-1};

	//file pointer for reading from save file
	FILE *fptr = fopen("save_file.txt", "r");

	//if we are loading from save data and 
	//save file exists then populate state variables
	//with data from file
	if (save_data == 1 && fptr != NULL) {
		for (int j = 0; j < grid_rows; j++) {
			for (int k = 0; k < grid_cols; k++) {
				fscanf(fptr, "%d,", &choices[j][k]);
			}
		}			
		fscanf(fptr, "%d", &score);
		fscanf(fptr, "%d", &i);
		fscanf(fptr, "%d,%d", &prev_choice[0], &prev_choice[1]);
	}
	else if (save_data == 0) {
		fptr = fopen("save_file.txt", "w");
		for (int j = 0; j < grid_rows; j++) {
			for (int k = 0; k < grid_cols; k++) {
				fscanf(fptr, "%d,", &choices[j][k]);
			}
		}			
		fscanf(fptr, "%d", &score);
		fscanf(fptr, "%d", &i);
		fscanf(fptr, "%d,%d", &prev_choice[0], &prev_choice[1]);
	}
	fclose(fptr);

	//declaring new window for containing grid
	WINDOW *grid_win;
	char instr1[] = "Use arrow keys to navigate the grid";
	char instr2[] = "Press enter when a selection has been made";
	char score_str[] = "Score: ";
	char fin_score_str[] = "Final Score: ";
	int highlighty = 0;
	int highlightx = 0;
	bool enter_pressed = false;
	bool q_pressed = false;
	int c;

	//variables for holding coordinates of 
	//where grid should be displayed on screen
	startx = (COLS - WIDTH) / 2;
	starty = (LINES - HEIGHT) / 2;

	grid_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(grid_win, TRUE);

	//printing information for player
	mvprintw(3, (COLS-strlen(instr1))/2, instr1);
	mvprintw(4, (COLS-strlen(instr2))/2, instr2);
	mvprintw(LINES - 5, (COLS-strlen(score_str))/2, score_str);
	printw("%d", score);
	mvprintw(LINES - 2, 0, "Press q to exit");
	refresh();

	//while loop for taking and reacting to user input
	while (i <= 25) {
		//if enter has been pressed, a cell has been selected
		//update 2d array with grid info
		//update previous choice coordinates
		if (enter_pressed) {
			choices[highlighty][highlightx] = i;
			prev_choice[0] = highlighty;
			prev_choice[1] = highlightx;
			print_grid(grid_win, highlighty, highlightx, grid_rows, grid_cols, choices);
			enter_pressed = false;
			//display new score
			mvprintw(LINES - 5, (COLS-strlen(score_str))/2, score_str);
			printw("%d", score);
			refresh();
		}
		else {
			//highlight cell where cursor is currently
			print_grid(grid_win, highlighty, highlightx, grid_rows, grid_cols, choices);
		}

		if (i == 25) {
			//when all numbers have been placed
			//display final score and end game
			mvprintw(LINES - 5, (COLS-strlen(fin_score_str))/2, fin_score_str);
			printw("%d", score);
			break;
		}

		//switch statement for interpreting user input
		c = wgetch(grid_win);
		switch (c) {
			//defining how movement works
			case KEY_UP:
				if (highlighty == 0)
					highlighty = grid_rows-1;
				else
					--highlighty;
				break;
			case KEY_DOWN:
				if (highlighty == grid_rows-1)
					highlighty = 0;
				else
					++highlighty;
				break;
			case KEY_LEFT:
				if (highlightx == 0)
					highlightx = grid_cols-1;
				else
					--highlightx;
				break;
			case KEY_RIGHT:
				if (highlightx == grid_cols-1)
					highlightx = 0;
				else
					++highlightx;
				break;
			//what happens when enter is pressed
			case 10:
				enter_pressed = true;
				break;
			//what happens when q is pressed
			case 113:
				q_pressed = true;				
				break;
			default:
				break;
		}
		
		//when q is pressed, player wants to quit
		//give option to save or not
		if (q_pressed) {
			mvprintw(LINES - 2, 0, "Save your progress? [y/n]");
			while (1) {
				c = getch();
				//if player enters y
				if (c == 121) {
					//pass state data to save_to_file function
					save_to_file(choices, score, i, prev_choice);
					return;
				}

				//if player enters n
				//exit without saving
				if (c == 110) {
					return;
				}	
			}
			break;
		}
		if (enter_pressed) {
			//if user enters in a square that has already been entered
			if (choices[highlighty][highlightx] != 0) {
				mvprintw(LINES-2, 0, "Game Over");
				break;
			}
			//logic that depends on previous choice
			if (prev_choice[0] != -1 && prev_choice[1] != -1) {
				//check if user has not entered their value in an adjacent space
				if (abs(highlighty - prev_choice[0]) > 1 || abs(highlightx - prev_choice[1]) > 1) {
					mvprintw(LINES-2, 0, "Game Over");
					break;
				}
				//check if a point has been earned
				if (abs(highlighty - prev_choice[0]) == 1 && abs(highlightx - prev_choice[1]) == 1) {
					score++;
				}
			}

			i++;
		}
	}

	clrtoeol();	
	refresh();
	getch();

	return ;
}

void print_grid(WINDOW *grid_win, int highlighty, int highlightx, int nrows, int ncols, int choices[nrows][ncols]) {
	int x, y, i, j;
	x = 2;
	y = 2;
	box(grid_win, 0, 0);
	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			if (highlightx == j && highlighty == i) {
				wattron(grid_win, A_REVERSE);
				//populate grid with contents of choices array
				if (choices[i][j] == 0)
					mvwprintw(grid_win, y, x, "_");
				else
					mvwprintw(grid_win, y, x, "%d", choices[i][j]);
				wattroff(grid_win, A_REVERSE);
			}
			else
				if (choices[i][j] == 0)
					mvwprintw(grid_win, y, x, "_");
				else
					mvwprintw(grid_win, y, x, "%d", choices[i][j]);
			x += 5;
		}
		x = 2;
		y += 5;
	}	
	wrefresh(grid_win);	
}
