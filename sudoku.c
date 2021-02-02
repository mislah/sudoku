// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Sudoku 2.0.0
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

void display(short[9][9]);
void sysdisp(short[9][9]);
void allinput(short[9][9]);
void edtinput(short[9][9]);
short chkcomp(short[9][9]);
int isallowed(short[9][9], int, int, int);
void genpuz(short[9][9], int);
void respuz(short[9][9], short[9][9]);
short chkwin(short[9][9]);
int solve(short[9][9], int, int);
void help(void);
void about(void);
int stpin; // flag: stop input

int main(void) {
	short A[9][9], puz[9][9];
	int n;
	do {
	mainmenu:
		fflush(stdout);
		system("clear");
		printf("1: Game\n2: Solver\n3: Help\n4: About\n5: Exit\nEnter your input : ");
		scanf(" %d", &n);
		int q;
		switch (n) {
		case 1:
		newgame:
			respuz(A, 0);
			respuz(puz, 0);
			fflush(stdout);
			system("clear");
			printf("1: Very Easy\n2: Easy\n3: Medium\n4: Hard\n5: Very Hard\nEnter your input : ");
			scanf(" %d", &q);
			switch (q) {
			case 1:
				genpuz(puz, 70);
				break;
			case 2:
				genpuz(puz, 50);
				break;
			case 3:
				genpuz(puz, 30);
				break;
			case 4:
				genpuz(puz, 25);
				break;
			case 5:
				genpuz(puz, 17);
				break;
			}
			respuz(A, puz);
			int opt;
			while (!chkwin(A)) {
				display(A);
				sysdisp(puz);
				printf("Enter 000 for menu\n");
				edtinput(A);
				if (stpin == 1) {
					do {
						display(A);
						sysdisp(puz);
						printf("1: Cancel\n2: New Puzzle\n3: View Solution\n4: Main Menu\n5: Quit\nEnter your input : ");
						scanf(" %d", &opt);
					} while (!(opt > 0 && opt < 6));
					if (opt == 3) {
						respuz(A, 0);
						respuz(A, puz);
						solve(A, 0, 0);
						char c;
						do {
							display(A);
							sysdisp(puz);
							printf("Enter q to quit : \n");
							scanf(" %c", &c);
						} while (c != 'q' && c != 'Q');
						goto mainmenu;
					}
					else if (opt == 2) {
						goto newgame;
					}
					else if (opt == 4) {
						goto mainmenu;
					}
					else if (opt == 5) {
						return 0;
					}
				}
				respuz(A, puz);
			}
			display(A);
			sysdisp(puz);
			printf("Congratulations! You won!");
			fflush(stdout);
			usleep(2000000);
			q = 0;
			break;
		case 2:
			respuz(A, 0);
			do {
				display(A);
				printf("1: All Input\n2: Edit\n3: Solve\n4: Reset\n5: Main Menu\n6: Exit\nEnter your input : ");
				scanf(" %d", &q);
				if (q == 1) {
					allinput(A);
				}
				else if (q == 2) {
					do {
						display(A);
						printf("Enter 000 to quit edit menu\n");
						edtinput(A);
					} while (stpin == 0);
				}
				else if (q == 3) {
					respuz(puz, 0);
					respuz(puz, A);
					solve(A, 0, 0);
					if (!chkwin(A)) {
						respuz(A, 0);
						respuz(A, puz);
						display(A);
						printf("No solution exists");
						fflush(stdout);
						usleep(2000000);
					}
				}
				else if (q == 4) {
					respuz(A, 0);
				}
				else if (q == 6) {
					return 0;
				}
			} while (q != 5);
			q = 0;
			break;
		case 3:
			help();
			break;
		case 4:
			about();
			break;
		}
	} while (n != 5);
quit:
	return 0;
}

void allinput(short A[9][9]) {
	char k;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			display(A);
			printf("Enter input for %c%d : \n", i + 'a', j + 1);
			scanf(" %c", &k);
			if (k - '0' < 0 || k - '0' > 9) {
				j--;
				if (j == -1) {
					j = 8;
					i--;
				}
			}
			else {
				A[i][j] = k - '0';
			}
		}
	}
}

void edtinput(short A[9][9]) {
	char i, j;
	int k;
	stpin = 0;
	printf("Enter the address and input : ");
	scanf(" %c %c %d", &i, &j, &k);
	if (i - '0' == 0 && j - '0' == 0 && k == 0) {
		stpin = 1;
		return;
	}
	if (i - 'a' > 8 || i - 'a' < 0 || j - '1' > 8 || j - '1' < 0) {
		return;
	}
	if (k > 9 || k < 0) {
		k = 0;
	}
	A[i - 'a'][j - '1'] = k;
}

int isallowed(short A[9][9], int m, int  n, int k) {
	for (int j = 0; j < 9; j++) {
		if (A[m][j] == k) {
			return 0;
		}
	}
	for (int i = 0; i < 9; i++) {
		if (A[i][n] == k) {
			return 0;
		}
	}
	for (int i = m - m % 3; i < m - m % 3 + 3; i++) {
		for (int j = n - n % 3; j < n - n % 3 + 3; j++) {
			if (A[i][j] == k) {
				return 0;
			}
		}
	}
	return 1;
}

void genpuz(short puz[9][9], int d) {
	int k, q = 0;
	srand(time(0));
	for (int j = 0; j < 9; j++) {
		if (puz[0][j] != 0) {
			q = 1;
		}
	}
	while (!q) {
		int z;
		k = (rand() % 9);
		z = (rand() % 9) + 1;
		if (puz[0][k] == 0) {
			if (isallowed(puz, 0, k, z)) {
				puz[0][k] = z;
				q = 1;
			}
		}
	}
	do {
		for (int i = 0; i < 4; i++) {
			while (1) {
				int a = rand() % 9;
				int b = rand() % 9;
				int c = (rand() % 9) + 1;
				if (puz[a][b] != 0) {
					if (isallowed(puz, a, b, c)) {
						puz[a][b] = c;
						break;
					}
				}
			}
		}
		k = solve(puz, 0, 0);
		for (int i = 0; i < 81 - d; i++) {
			int a = rand() % 9;
			int b = rand() % 9;
			if (puz[a][b] != 0) {
				puz[a][b] = 0;
			}
			else {
				i--;
			}
		}
	} while (k != 1);
}

void respuz(short A[9][9], short puz[9][9]) {
	if (puz == 0) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				A[i][j] = 0;
			}
		}

	}
	else {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (puz[i][j] != 0) {
					A[i][j] = puz[i][j];
				}
			}
		}
	}
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
			if (j == 8 && A[i][j] != k) {
				return 0;
			}
			if (A[i][j] == k) {
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
			if (i == 8 && A[i][j] != k) {
				return 0;
			}
			if (A[i][j] == k) {
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
					if (i == m - 1 && j == n - 1 && A[i][j] != k) {
						return 0;
					}
					if (A[i][j] == k) {
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
	do {
		fflush(stdout);
		system("clear");
		printf("\n Psst, ever heard of this enchanting realm called the internet?\n It's a place where you can find answers to all your questions.\n");
		printf("\n Enter q to quit : ");
		scanf(" %c", &c);
	} while (c != 'q' && c != 'Q');
}

void about(void) {
	char c;
	do {
		fflush(stdout);
		system("clear");
		printf("\n Sudoku v2.0.0\n\n Developed by Mislah Rahman.\n");
		printf("\n Enter q to quit : ");
		scanf(" %c", &c);
	} while (c != 'q' && c != 'Q');
}

void sysdisp(short A[9][9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				continue;
			}
			char Bold[] = { "𝟬" };
			Bold[3] += A[i][j];
			printf("\e[%d;%df%s", 3 + 2 * i, 5 + 4 * j, Bold);
		}
	}
	printf("\e[%d;%df", 22, 0);
}

void display(short A[9][9]) {
	fflush(stdout);
	system("clear");
	printf("    1   2   3   4   5   6   7   8   9 \n  ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\na ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nb ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nc ║   │   │   ║   │   │   ║   │   │   ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\nd ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\ne ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nf ║   │   │   ║   │   │   ║   │   │   ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\ng ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nh ║   │   │   ║   │   │   ║   │   │   ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\ni ║   │   │   ║   │   │   ║   │   │   ║\n  ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n\n");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (A[i][j] == 0) {
				continue;
			}
			printf("\e[%d;%df%hu", 3 + 2 * i, 5 + 4 * j, A[i][j]);
		}
	}
	printf("\e[%d;%df", 22, 0);
}
