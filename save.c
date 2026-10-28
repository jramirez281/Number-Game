#include "Ass1.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void save_to_file(int choices[5][5], int score, int last_num, int prev_choice[2]);
bool save_exists();

//function for storing save state data of current game in a text file
void save_to_file(int choices[5][5], int score, int last_num, int prev_choice[2]) {
	FILE *fptr;
	//save file format: 1st row contents of choices array, 2nd row score
	//3rd row last number placed, 4th row coordinates of previous choice
	fptr = fopen("save_file.txt", "w");

	if (fptr == NULL) {
		mvprintw(LINES - 2, 0, "An error occurred");
		return;
	}
	
	int i, j;	
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			fprintf(fptr, "%d,", choices[i][j]);
		}
	}

	fprintf(fptr, "\n%d\n%d\n", score, last_num);
	fprintf(fptr, "%d,%d", prev_choice[0], prev_choice[1]);
	
	fclose(fptr);
	return;
}

//function for confirming existence of save file
bool save_exists() {
	bool file_exists = true;
	FILE *fptr;
	fptr = fopen("save_file.txt", "r");

	if (fptr == NULL)
		file_exists = false;

	fclose(fptr);
	return file_exists;
}
