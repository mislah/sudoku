// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Sudoku 5.2.1
 * Copyright 2021 Mislah Rahman.
 * Author: Mislah Rahman
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <limits.h>

struct highscore {
	int score[5];
	char name[5][21];
};
void display(short[9][9]);
void genpuz(short[9][9], short);
void respuz(short[9][9], short);
short chkcomp(short[9][9]);
short chksolvable(short[9][9]);
short isallowed(short[9][9], short, short, short);
short solve(short[9][9], short, short);
short edit(short[9][9], short, short*, short*);
short getin(void);
void help(void);
void about(void);
void prinths(short);
void writehs(short, int);

int main(void) {
	short A[9][9], n;
	struct termios def, off;
	tcgetattr(STDIN_FILENO, &def);
	off = def;
	off.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &off);
	printf("\e[8;22;80t\e[?25l");
	do {
	mainmenu:
		fflush(stdout);
		system("clear");
		printf("1: Game\n2: Solver\n3: Help\n4: Highscore\n5: About\n6: Exit");
		n = getin();
		short q, opt, x = 0, y = 0;
		switch (n) {
		case 1:
		newgame:
			respuz(A, 0);
			do {
				fflush(stdout);
				system("clear");
				printf("New Game\n1: Easy\n2: Medium\n3: Hard\n4: Extreme\nq: Main Menu");
				q = getin();
			} while (!(q >= 1 && q <= 4 || q == -2));
			long tstart, ttaken;
			time(&tstart);
			switch (q) {
			case 1:
				genpuz(A, 60);
				break;
			case 2:
				genpuz(A, 45);
				break;
			case 3:
				genpuz(A, 30);
				break;
			case 4:
				genpuz(A, 22);
				break;
			case -2:
				goto mainmenu;
			}
			while (1) {
				display(A);
				if (edit(A, 1, &x, &y)) {
					display(A);
					time(&ttaken);
					ttaken -= tstart;
					printf("\e[8;44fTime taken: %ld mins %ld sec", ttaken / 60, ttaken % 60);
					printf("\e[9;44f1: Clear Input\e[10;44f2: View Solution\e[11;44f3: New Puzzle\e[12;44f4: Main Menu\e[13;44f5: Quit");
					opt = getin();
					switch (opt) {
					case 1:
						respuz(A, 1);
						break;
					case 2:
						respuz(A, 1);
						solve(A, 0, 0);
						display(A);
						while (getin() != -2);
						goto mainmenu;
					case 3:
						goto newgame;
					case 4:
						goto mainmenu;
					case 5:
						goto end;
					}
				}
				else {
					break;
				}
			}
			time(&ttaken);
			ttaken -= tstart;
			display(A);
			printf("\e[11;44fCongratulations! You won!");
			printf("\e[12;44fTime taken: %ld mins %ld sec", ttaken / 60, ttaken % 60);
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &def);
			writehs(q, (int)ttaken);
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &off);
			getin();
			fflush(stdout);
			break;
		case 2:
			respuz(A, 0);
			while (1) {
				display(A);
				if (edit(A, 0, &x, &y)) {
					printf("\e[8;44f1: Solve\e[9;44f2: Reset\e[10;44f3: Main Menu\e[11;44f4: Exit");
					opt = getin();
					switch (opt) {
					case 1:
						respuz(A, 2);
						if (!chksolvable(A) || !solve(A, 0, 0)) {
							respuz(A, 1);
							respuz(A, 3);
							display(A);
							printf("\e[11;44fNo solution exists!");
							fflush(stdout);
							usleep(2000000);
						}
						else {
							display(A);
							printf("\e[21;9fPress q to edit the grid");
							while (getin() != -2);
						}
						break;
					case 2:
						respuz(A, 0);
						break;
					case 3:
						goto mainmenu;
					case 4:
						goto end;
					}
				}
				respuz(A, 3);
			}
			break;
		case 3:
			help();
			break;
		case 4:
			while (1) {
				do {
					fflush(stdout);
					system("clear");
					printf("Highscores\n1: Easy\n2: Medium\n3: Hard\n4: Extreme\nq: Main Menu");
					q = getin();
				} while (!(q >= 1 && q <= 4 || q == -2));
				if (q == -2) {
					break;
				}
				prinths(q);
				getin();
			}
			break;
		case 5:
			about();
			break;
		}
	} while (n != 6);
end:
	printf("\e[?25h");
	fflush(stdout);
	system("clear");
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &def);
	return 0;
}

/*
 * Edits the content of the current cell if it is editable
 * x, y contains current position of cursor on the grid
 * Returns 0 if the user wins the game and if chk==1
 */
short edit(short A[9][9], short chk, short* x, short* y) {
	short in, i, j;
	printf("\e[?25h");
	fflush(stdout);
	for (i = *x; i < 9; i++) {
		for (j = *y; j < 9;) {
			printf("\e[%d;%df", 3 + 2 * i, 5 + 4 * j);
			fflush(stdout);
			in = getin();
			if (in == -2) {
				*x = i;
				*y = j;
				printf("\e[?25l");
				return 1;
			}
			else if (in < 10 && in != -1 && A[i][j] < 10) {
				A[i][j] = in;
				if (in != 0) {
					char Bold[] = { "𝟬" };
					Bold[3] += in;
					printf("%s", Bold);
				}
				if (in == 0) {
					printf(" ");
				}
				fflush(stdout);
			}
			else if (in == 11) {
				if (i != 0) {
					i--;
				}
			}
			else if (in == 22) {
				if (i != 8) {
					i++;
				}
			}
			else if (in == 33) {
				if (!(i == 8 && j == 8)) {
					j++;
				}
			}
			else if (in == 44) {
				if (j != 0) {
					j--;
				}
				else if (i != 0) {
					j = 8;
					i--;
				}
				else {
					j = 0;
				}
			}
			if (chk == 1 && chkcomp(A) == 1) {
				if (chksolvable(A)) {
					printf("\e[?25l");
					return 0;
				}
			}
		}
	}
	return 0;
}

/*
 * Gets input from the user including arrow key inputs
 */
short getin(void) {
	char c;
	fflush(stdout);
	if (read(STDIN_FILENO, &c, 1) == 1) {
		if (c == '\e') {
			char seq[2];
			if (read(STDIN_FILENO, &seq[0], 1) != 1) {
				return -1;
			}
			if (read(STDIN_FILENO, &seq[1], 1) != 1) {
				return -1;
			}
			if (seq[0] == '[') {
				switch (seq[1]) {
				case 'A': // Up Arrow
					return 11;
				case 'B': // Down Arrow
					return 22;
				case 'C': // Right Arrow				
					return 33;
				case 'D': // Left Arrow
					return 44;
				}
			}
		}
		else if (c == 'q' || c == 'Q') {
			return -2;
		}
		else if (c - '0' >= 0 && c - '0' <= 9) {
			return c - '0';
		}
		else {
			return 0;
		}
	}
	return 0;
}

/*
 * Returns 1 if k is allowed in the mxn cell, returns 0 otherwise
 */
short isallowed(short A[9][9], short m, short  n, short k) {
	short i, j;
	for (i = 0; i < 9; i++) {
		if ((A[i][n] == k || A[i][n] - 10 == k) & m != i) {
			return 0;
		}
		if ((A[m][i] == k || A[m][i] - 10 == k) & n != i) {
			return 0;
		}
	}
	for (i = m - m % 3; i < m - m % 3 + 3; i++) {
		for (j = n - n % 3; j < n - n % 3 + 3; j++) {
			if ((A[i][j] == k || A[i][j] - 10 == k) && !(m == i && n == j)) {
				return 0;
			}
		}
	}
	return 1;
}

/*
 * Generates a puzzle with d cells filled by the system
 */
void genpuz(short A[9][9], short d) {
	short r[9], z = 0, tmp, i, j, k;
	srand(time(0));
	for (i = 0; i < 9; i++) {
		r[i] = i + 1;
	}
	do {
		for (i = 9; i > 0; i--) {
			k = rand() % i;
			tmp = r[i - 1];
			r[i - 1] = r[k];
			r[k] = tmp;
		}
		k = 0;
		for (i = z; i < 3 + z; i++) {
			for (j = z; j < 3 + z; j++) {
				A[i][j] = r[k];
				k++;
			}
		}
		z += 3;
	} while (z != 9);
	solve(A, 0, 0);
	for (i = 0; i < 81 - d; i++) {
		short a = rand() % 9;
		short b = rand() % 9;
		if (A[a][b] != 0) {
			A[a][b] = 0;
		}
		else {
			i--;
		}
	}
	respuz(A, 2);
}

/*
 * Does some basic operations on the grid matrix
 * Mode 0: Clears all values of the grid
 * Mode 1: Clears all user inputs of the grid
 * Mode 2: Upgrades all values to system input
 * Mode 3: Downgrades all values to user input
 */
void respuz(short A[9][9], short mode) {
	short i, j;
	switch (mode) {
	case 0:
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				A[i][j] = 0;
			}
		}
		break;
	case 1:
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (A[i][j] < 10) {
					A[i][j] = 0;
				}
			}
		}
		break;
	case 2:
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (A[i][j] != 0) {
					A[i][j] += 10;
				}
			}
		}
		break;
	case 3:
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (A[i][j] > 10) {
					A[i][j] -= 10;
				}
			}
		}
		break;
	}
}

/*
 * Returns 1 if the grid is a valid sudoku puzzle, returns 0 otherwise
 */
short chksolvable(short A[9][9]) {
	short a, i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (A[i][j] != 0) {
				if (!isallowed(A, i, j, A[i][j])) {
					return 0;
				}
			}
		}
	}
	return 1;
}

/*
 * Solves the current grid. i, j should be 0
 */
short solve(short A[9][9], short i, short j) {
	if (i == 8 && j == 9) {
		return 1;
	}
	if (j == 9) {
		i++;
		j = 0;
	}
	if (A[i][j] > 0) {
		return solve(A, i, j + 1);
	}
	for (short n = 1; n <= 9; n++) {
		if (isallowed(A, i, j, n)) {
			A[i][j] = n;
			if (solve(A, i, j + 1)) {
				return 1;
			}
		}
		A[i][j] = 0;
	}
	return 0;
}

/*
 * Return 1 if all cells are filled, returns 0 otherwise
 */
short chkcomp(short A[9][9]) {
	short i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				return 0;
			}
		}
	}
	return 1;
}

/*
 * Reads and prints highscores from the file:/sudoku.bin
 * Argument n is to choose the difficulty level, n=[1,4]
 */
void prinths(short n) {
	n--;
	fflush(stdout);
	system("clear");
	struct highscore hs;
	FILE* fptr;
	if ((fptr = fopen("sudoku.bin", "r")) == NULL) {
		printf("No records!");
		return;
	}
	fseek(fptr, sizeof(struct highscore) * n, SEEK_SET);
	fread(&hs, sizeof(struct highscore), 1, fptr);
	fclose(fptr);
	if (hs.score[0] == INT_MAX) {
		printf("No records!");
		return;
	}
	for (short i = 0; i < 5; i++) {
		if (hs.score[i] == INT_MAX) {
			return;
		}
		printf("%d. %dmin %dsec %s", i + 1, hs.score[i] / 60, hs.score[i] % 60, hs.name[i]);
	}
}

/*
 * Writes the highscore to file:/sudoku.bin
 * Argument n is to choose the difficulty level, n=[1,4]
 */
void writehs(short n, int score) {
	n--;
	struct highscore d[4];
	short i;
	char name[21];
	FILE* fptr;
	if ((fptr = fopen("sudoku.bin", "rb")) == NULL) {
		fptr = fopen("sudoku.bin", "wb");
		for (i = 0; i < 4; i++) {
			for (short j = 0; j < 5; j++) {
				d[i].score[j] = INT_MAX;
				d[i].name[j][0] = '\0';
			}
			fwrite(&d[i], sizeof(struct highscore), 1, fptr);
		}
		fptr = fopen("sudoku.bin", "rb");
	}
	fread(&d[0], sizeof(struct highscore), 4, fptr);
	for (i = 0; i < 5; i++) {
		if (d[n].score[i] > score) {
			printf("\e[?25h\e[13;44f");
			if (!i) {
				printf("New Highscore!\e[14;44f");
			}
			printf("Enter your name : ");
			fgets(name, 21, stdin);
			printf("\e[?25l");
			for (; i < 5; i++) {
				d[n].score[4] = d[n].score[i];
				d[n].score[i] = score;
				score = d[n].score[4];
				strcpy(d[n].name[4], d[n].name[i]);
				strcpy(d[n].name[i], name);
				strcpy(name, d[n].name[4]);
			}
			fptr = fopen("sudoku.bin", "wb");
			fwrite(&d[0], sizeof(struct highscore), 4, fptr);
			fclose(fptr);
			prinths(n + 1);
			break;
		}
	}
}

/*
 * Prints the help menu
 */
void help(void) {
	char c;
	fflush(stdout);
	system("clear");
	printf("\n\
   Sudoku\n\
   \n\
   Game Objective:\n\
   - To fill the 9×9 grid with numbers so that each column, each row, and each\n\
     of the nine 3×3 subgrids contain all of the numbers from 1 to 9.\n\
   - Each row, each column and each subgrid can't have any numbers repeating.\n\
   \n\
   Sudoku Solver:\n\
   - Input an unsolved sudoku puzzle and select solve option to get the\n\
     solution if the input grid is a valid sudoku puzzle.\n\
   \n\
   Input Methods:\n\
   - Use arrow keys to move through the grid and input numbers from 1 to 9\n\
   - Any other keys deletes the current value of the cell.\n\
   - The button q (or Q) open/close menu while playing the game or while using\n\
     the sudoku solver.\n\
   - Most of the menus and submenus exit upon pressing any key, press q (or Q)\n\
     if it doesn't.\n\
   \n\
   ");
	fflush(stdout);
	read(STDIN_FILENO, &c, 1);
}

/*
 * Prints the license and copyright information
 */
void about(void) {
	char c;
	fflush(stdout);
	system("clear");
	printf("\n\
   Sudoku v5.2.1\n\
   \n\
   Copyright 2021 Mislah Rahman.\n\
   Author: Mislah Rahman\n\
   License: GNU GPL 3.0 or later\n\
   \n\
   This program is free software: you can redistribute it and/or modify\n\
   it under the terms of the GNU General Public License as published by\n\
   the Free Software Foundation, either version 3 of the License, or\n\
   (at your option) any later version.\n\
   \n\
   This program is distributed in the hope that it will be useful,\n\
   but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
   GNU General Public License for more details.\n\
   \n\
   You should have received a copy of the GNU General Public License\n\
   along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\
   \n\
   ");
	fflush(stdout);
	read(STDIN_FILENO, &c, 1);
}

/*
 * Draws the sudoku grid and displays the content of grid matrix
 * Both system inputs and user inputs are bolded and colored, but with different colors
 */
void display(short A[9][9]) {
	fflush(stdout);
	system("clear");
	printf("\e[34m\n  ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n\n\e[m");
	for (short i = 0; i < 9; i++) {
		for (short j = 0; j < 9; j++) {
			if (!A[i][j]) {
				continue;
			}
			char Bold[] = { "𝟬" };
			if (A[i][j] < 10) {
				Bold[3] += A[i][j];
				printf("\e[%d;%df%s", 3 + 2 * i, 5 + 4 * j, Bold);
			}
			else {
				Bold[3] += A[i][j] - 10;
				printf("\e[32m\e[%d;%df%s\e[m", 3 + 2 * i, 5 + 4 * j, Bold);
			}
		}
	}
	printf("\e[22;0f");
}
