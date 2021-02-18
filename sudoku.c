// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Sudoku 4.0.0
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

void display(short[9][9]);
short chkcomp(short[9][9]);
int isallowed(short[9][9], int, int, int);
void genpuz(short[9][9], int);
void respuz(short[9][9], int);
short chkwin(short[9][9]);
int chksolvable(short[9][9]);
int solve(short[9][9], int, int);
int getin();
int edit(short[9][9], int);
void help(void);
void about(void);

int main(void) {
	short A[9][9];
	char n;
	struct termios def, off;
	tcgetattr(STDIN_FILENO, &def);
	off = def;
	off.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &off);
	printf("\e[?25l");
	do {
	mainmenu:
		fflush(stdout);
		system("clear");
		printf("1: Game\n2: Solver\n3: Help\n4: About\n5: Exit");
		fflush(stdout);
		read(STDIN_FILENO, &n, 1);
		char q;
		q = '0';
		switch (n - '0') {
		case 1:
		newgame:
			respuz(A, 0);
			do {
				fflush(stdout);
				system("clear");
				printf("1: Easy\n2: Medium\n3: Hard\n4: Extreme");
				fflush(stdout);
				read(STDIN_FILENO, &q, 1);
			} while (q - '0' < 1 || q - '0' > 4 && q != 'q' && q != 'Q');
			long tstart, ttaken;
			time(&tstart);
			switch (q - '0') {
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
			case 'q' - '0':
			case 'Q' - '0':
				goto mainmenu;
			}
			char opt;
			while (1) {
				display(A);
				if (edit(A, 1)) {
					do {
						display(A);
						time(&ttaken);
						ttaken -= tstart;
						printf("\e[8;44fTime taken: %ld mins %ld sec\e[9;44f1: Edit\e[10;44f2: Clear Input\e[11;44f3: View Solution\e[12;44f4: New Puzzle\e[13;44f5: Main Menu\e[14;44f6: Quit", ttaken / 60, ttaken % 60);
						fflush(stdout);
						read(STDIN_FILENO, &opt, 1);
					} while (!(opt - '0' > 0 && opt - '0' < 7));
					if (opt - '0' == 2) {
						respuz(A, 1);
					}
					else if (opt - '0' == 3) {
						respuz(A, 1);
						solve(A, 0, 0);
						char c;
						do {
							display(A);
							fflush(stdout);
							read(STDIN_FILENO, &c, 1);
						} while (c != 'q' && c != 'Q');
						goto mainmenu;
					}
					else if (opt - '0' == 4) {
						goto newgame;
					}
					else if (opt - '0' == 5) {
						goto mainmenu;
					}
					else if (opt - '0' == 6) {
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
			fflush(stdout);
			usleep(3000000);
			break;
		case 2:
			respuz(A, 0);
			while (1) {
				display(A);
				printf("\e[8;44f1: Edit\e[9;44f2: Solve\e[10;44f3: Reset\e[11;44f4: Main Menu\e[12;44f5: Exit");
				fflush(stdout);
				read(STDIN_FILENO, &q, 1);
				switch (q - '0') {
				case 1:
					respuz(A, 4);
					display(A);
					edit(A, 0);
					break;
				case 2:
					respuz(A, 3);
					if (!chksolvable(A) || !solve(A, 0, 0)) {
						respuz(A, 1);
						respuz(A, 4);
						display(A);
						printf("\e[11;44fNo solution exists!");
						fflush(stdout);
						usleep(2000000);
					}
					break;
				case 3:
					respuz(A, 0);
					break;
				case 4:
					goto mainmenu;
				case 5:
					goto end;
				}
			}
			break;
		case 3:
			help();
			break;
		case 4:
			about();
			break;
		}
	} while (n != '5');
end:
	printf("\e[?25h");
	fflush(stdout);
	system("clear");
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &def);
	return 0;
}

int edit(short A[9][9], int chk) {
	printf("\e[?25h");
	int in, i, j;
	fflush(stdout);
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9;) {
			printf("\e[%d;%df", 3 + 2 * i, 5 + 4 * j);
			fflush(stdout);
			in = getin();
			if (in == -2) {
				printf("\e[?25l");
				return 1;
			}
			else if (in < 10 && in != -1 && A[i][j] < 10) {
				A[i][j] = in;
				if (in != 0) {
					printf("%d", in);
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
				if (chkwin(A)) {
					printf("\e[?25l");
					return 0;
				}
			}
		}
	}
}

int getin() {
	char c;
	if (read(STDIN_FILENO, &c, 1) == 1) {
		if (c == '\e') {
			char seq[3];
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
	}
	return -1;
}

int isallowed(short A[9][9], int m, int  n, int k) {
	for (int i = 0; i < 9; i++) {
		if (A[i][n] == k || A[i][n] - 10 == k) {
			return 0;
		}
		if (A[m][i] == k || A[m][i] - 10 == k) {
			return 0;
		}
	}
	for (int i = m - m % 3; i < m - m % 3 + 3; i++) {
		for (int j = n - n % 3; j < n - n % 3 + 3; j++) {
			if (A[i][j] == k || A[i][j] - 10 == k) {
				return 0;
			}
		}
	}
	return 1;
}

void genpuz(short A[9][9], int d) {
	int r[9], z = 0, tmp, i, j, k;
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
	for (int i = 0; i < 81 - d; i++) {
		int a = rand() % 9;
		int b = rand() % 9;
		if (A[a][b] != 0) {
			A[a][b] = 0;
		}
		else {
			i--;
		}
	}
	respuz(A, 3);
}

void respuz(short A[9][9], int mode) {
	int i, j;
	// Clear
	if (mode == 0) {
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				A[i][j] = 0;
			}
		}

	}
	// Clear L0
	else if (mode == 1) {
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (A[i][j] < 10) {
					A[i][j] = 0;
				}
			}
		}
	}
	// Switch L1
	else if (mode == 3) {
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (A[i][j] != 0) {
					A[i][j] += 10;
				}
			}
		}
	}
	// Switch L0
	else if (mode == 4) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (A[i][j] > 10) {
					A[i][j] -= 10;
				}
			}
		}
	}
}

int chksolvable(short A[9][9]) {
	int a;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] != 0) {
				a = A[i][j];
				A[i][j] = 0;
				if (!isallowed(A, i, j, a)) {
					A[i][j] = a;
					return 0;
				}
				else {
					A[i][j] = a;
				}
			}
		}
	}
	return 1;
}

int solve(short A[9][9], int m, int n) {
	if (m == 8 && n == 9) {
		return 1;
	}
	if (n == 9) {
		m++;
		n = 0;
	}
	if (A[m][n] > 0) {
		return solve(A, m, n + 1);
	}
	for (int num = 1; num <= 9; num++) {
		if (isallowed(A, m, n, num) == 1) {
			A[m][n] = num;
			if (solve(A, m, n + 1) == 1) {
				return 1;
			}
		}
		A[m][n] = 0;
	}
	return 0;
}

short chkwin(short A[9][9]) {
	int i, j, k, m, n;
	if (!chkcomp(A)) {
		return 0;
	}
	for (i = 0; i < 9; i++) {
		k = 1;
		for (j = 0; j < 9; j++) {
			if (j == 8 && (A[i][j] != k && A[i][j] - 10 != k)) {
				return 0;
			}
			if (A[i][j] == k || A[i][j] - 10 == k) {
				k++;
				if (k == 10) {
					break;
				}
				j = -1;
			}
		}
	}
	for (j = 0; j < 9; j++) {
		k = 1;
		for (i = 0; i < 9; i++) {
			if (i == 8 && (A[i][j] != k && A[i][j] - 10 != k)) {
				return 0;
			}
			if (A[i][j] == k || A[i][j] - 10 == k) {
				k++;
				if (k == 10) {
					break;
				}
				i = -1;
			}
		}
	}
	for (m = 3; m < 10; m = m + 3) {
		for (n = 3; n < 10; n = n + 3) {
			k = 1;
			for (i = m - 3; i < m; i++) {
				for (j = n - 3; j < n; j++) {
					if (i == m - 1 && j == n - 1 && (A[i][j] != k && A[i][j] - 10 != k)) {
						return 0;
					}
					if (A[i][j] == k || A[i][j] - 10 == k) {
						k++;
						if (k == 10) {
							break;
						}
						i = m - 3;
						j = n - 4;
					}
				}
				if (k == 10) {
					break;
				}
			}
		}
	}
	return 1;
}

short chkcomp(short A[9][9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				return 0;
			}
		}
	}
	return 1;
}

void help(void) {
	char c;
	fflush(stdout);
	system("clear");
	printf("\n Psst, ever heard of this enchanting realm called the internet?\n It's a place where you can find answers to all your questions.\n");
	printf("\n Ah, forgot to say, you must be searching for the 'q' key.\n");
	fflush(stdout);
	read(STDIN_FILENO, &c, 1);
}

void about(void) {
	char c;
	fflush(stdout);
	system("clear");
	printf("\n Sudoku v4.0.0\n\n Developed by Mislah Rahman.\n");
	printf("\n Press q to quit : ");
	fflush(stdout);
	read(STDIN_FILENO, &c, 1);
}

void display(short A[9][9]) {
	fflush(stdout);
	system("clear");
	printf("\n  ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n  ║   │   │   ║   │   │   ║   │   │   ║\n  ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n\n");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				continue;
			}
			if (A[i][j] < 10) {
				printf("\e[%d;%df%hu", 3 + 2 * i, 5 + 4 * j, A[i][j]);
			}
			else {
				char Bold[] = { "𝟬" };
				Bold[3] += A[i][j] - 10;
				printf("\e[%d;%df%s", 3 + 2 * i, 5 + 4 * j, Bold);
			}
		}
	}
	printf("\e[22;0f");
}
