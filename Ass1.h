#include <stdbool.h>
#include <curses.h>
void print_grid(WINDOW *grid_win, int highlighty, int highlightx, int nrows, int ncols, int choices[nrows][ncols]);
void play_game(int save_data);
void print_rules();
void save_to_file(int choices[5][5], int score, int last_num, int prev_choice[2]);
bool save_exists();
