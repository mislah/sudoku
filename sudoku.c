// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Sudoku 0.1.0
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

void display(short A[9][9]);
void edtinput(short A[9][9]);
short chkwin(short A[9][9]);
int stpin = 0; // flag: stop input

int main() {
	short A[9][9] = { 0 };
	while (stpin == 0) {
		display(A);
		printf("Enter 000 after entering puzzle to start playing\n");
		edtinput(A);
	}
	stpin = 0;
	while (!chkwin(A)) {
		display(A);
		edtinput(A);
		if (stpin == 1) {
			return 0;
		}
	}
	display(A);
	printf("Congratulations! You won!\n");
	fflush(stdout);
	usleep(2000000);
	return 0;
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

short chkwin(short A[9][9]) {
	int i, j, k, m, n;
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

void display(short A[9][9]) {
	fflush(stdout);
	system("clear");
	char B[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			B[i][j] = A[i][j] + '0';
			if (B[i][j] == '0') {
				B[i][j] = ' ';
			}
		}
	}
	printf("    1   2   3   4   5   6   7   8   9 \n  ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\na ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nb ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nc ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\nd ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\ne ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nf ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\ng ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\nh ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\ni ║ %c │ %c │ %c ║ %c │ %c │ %c ║ %c │ %c │ %c ║\n  ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n\n", B[0][0], B[0][1], B[0][2], B[0][3], B[0][4], B[0][5], B[0][6], B[0][7], B[0][8], B[1][0], B[1][1], B[1][2], B[1][3], B[1][4], B[1][5], B[1][6], B[1][7], B[1][8], B[2][0], B[2][1], B[2][2], B[2][3], B[2][4], B[2][5], B[2][6], B[2][7], B[2][8], B[3][0], B[3][1], B[3][2], B[3][3], B[3][4], B[3][5], B[3][6], B[3][7], B[3][8], B[4][0], B[4][1], B[4][2], B[4][3], B[4][4], B[4][5], B[4][6], B[4][7], B[4][8], B[5][0], B[5][1], B[5][2], B[5][3], B[5][4], B[5][5], B[5][6], B[5][7], B[5][8], B[6][0], B[6][1], B[6][2], B[6][3], B[6][4], B[6][5], B[6][6], B[6][7], B[6][8], B[7][0], B[7][1], B[7][2], B[7][3], B[7][4], B[7][5], B[7][6], B[7][7], B[7][8], B[8][0], B[8][1], B[8][2], B[8][3], B[8][4], B[8][5], B[8][6], B[8][7], B[8][8]);
}
