// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Sudoku 0.0.0
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

void display(short[9][9]);

int main() {
	system("clear");
	short A[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			A[i][j] = rand() % 9;
		}
	}
	display(A);
	return 0;
}

void display(short B[9][9]) {
	char A[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			A[i][j] = B[i][j] + '0';
			if (A[i][j] == '0') {
				A[i][j] = ' ';
			}
		}
	}
	printf("    1   2   3   4   5   6   7   8   9\n  \u2554\u2550\u2550\u2550\u2564\u2550\u2550\u2550\u2564\u2550\u2550\u2550\u2566\u2550\u2550\u2550\u2564\u2550\u2550\u2550\u2564\u2550\u2550\u2550\u2566\u2550\u2550\u2550\u2564\u2550\u2550\u2550\u2564\u2550\u2550\u2550\u2557\na \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255F\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2562\nb \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255F\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2562\nc \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u2560\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u2563\nd \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255F\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2562\ne \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255F\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2562\nf \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u2560\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u256A\u2550\u2550\u2550\u2563\ng \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255F\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2562\nh \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255F\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u256B\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2562\ni \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551 %c \u2502 %c \u2502 %c \u2551\n  \u255A\u2550\u2550\u2550\u2567\u2550\u2550\u2550\u2567\u2550\u2550\u2550\u2569\u2550\u2550\u2550\u2567\u2550\u2550\u2550\u2567\u2550\u2550\u2550\u2569\u2550\u2550\u2550\u2567\u2550\u2550\u2550\u2567\u2550\u2550\u2550\u255D\n", A[0][0], A[0][1], A[0][2], A[0][3], A[0][4], A[0][5], A[0][6], A[0][7], A[0][8], A[1][0], A[1][1], A[1][2], A[1][3], A[1][4], A[1][5], A[1][6], A[1][7], A[1][8], A[2][0], A[2][1], A[2][2], A[2][3], A[2][4], A[2][5], A[2][6], A[2][7], A[2][8], A[3][0], A[3][1], A[3][2], A[3][3], A[3][4], A[3][5], A[3][6], A[3][7], A[3][8], A[4][0], A[4][1], A[4][2], A[4][3], A[4][4], A[4][5], A[4][6], A[4][7], A[4][8], A[5][0], A[5][1], A[5][2], A[5][3], A[5][4], A[5][5], A[5][6], A[5][7], A[5][8], A[6][0], A[6][1], A[6][2], A[6][3], A[6][4], A[6][5], A[6][6], A[6][7], A[6][8], A[7][0], A[7][1], A[7][2], A[7][3], A[7][4], A[7][5], A[7][6], A[7][7], A[7][8], A[8][0], A[8][1], A[8][2], A[8][3], A[8][4], A[8][5], A[8][6], A[8][7], A[8][8]);
}
