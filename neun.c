
/* ------NEUN - A Text Editor------
 *Copyright (C) 2016  KAJAL SANKLECHA (111503030)

 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.

 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "neun.h"

position norm, max;
int line_no;

/*any_screen -> to initialise the keyboard keys in all the functions
 */

void any_screen() {
	keypad(stdscr, 1);
	clear();
	refresh();
}

/*key_up -> Up arrow key on the keyboard.
 *To move the cursor on the screen and off the screen one line up.
 */

line *key_up(file *fptr) {
	if(line_no == 0)
		return fptr->first;
	line *move = fptr->first;
	int i;
	move = fptr->first;
	for(i = 0; i < line_no; i++)
		move = move->next;
	if(move->prev == NULL) {
		return move;
	}
	if(norm.y == 0 && line_no != 1) {
		scrl(max.y / 2);
		move(norm.y = (max.y / 2 - 1), 0);
	}
	if(norm.x >= move->prev->chars) {
		move(--norm.y, norm.x = move->prev->chars - 1);
	}
	else
		move(--norm.y,norm.x);
	line_no--;
	return move;
}

/*key_down -> Down arrow key on the keyboard.
 *To move the cursor on the screen and off the screen one line down.
 */

line *key_down(file *fptr) {
	line *move;
	move = fptr->first;
	int i;
	for(i = 0; i < line_no; i++)
		move = move->next;
	if(line_no == fptr->lines	)
		return move;
	if(norm.y >= max.y) {
		scrl(-(max.y / 2));
		move(norm.y = (max.y / 2 - 1), 0);
	}
	if(norm.x >= move->next->chars) {
		move(++norm.y, norm.x = move->next->chars - 1);
	}
	else
		move(++norm.y,norm.x);
	line_no++;
	return move;
}

/*key_left -> Left arrow key on the keyboard.
 *To move the cursor on the screen and off the screen one character left.
 */

line *key_left(file *fptr) {
	line *move = fptr->first;
	int i;
	for(i = 0; i < line_no; i++) {
		move = move->next;
	}
	if(move->prev == NULL && norm.x == 0) {
		return move;
	}
	if(norm.x == 0) {
		move = move->prev;
		line_no--;
		norm.x = move->chars;
		move(--norm.y, norm.x);
		return move;
	}
	move(norm.y, --norm.x);
	return move;
}

/*key_right -> Right arrow key on the keyboard.
 *To move the cursor on the screen and off the screen one character right.
 */

line *key_right(file *fptr) {
	line *move = fptr->first;
	int i;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	if(norm.x >= move->chars) {
		if(line_no >= fptr->lines)
			return move;
		move = move->next;
		line_no++;
		move(++norm.y, norm.x = 0);
		return move;
	}
	move(norm.y, ++norm.x);
	return move;
}

/* homepage -> the first page of the editor
 */

void homepage() {
	any_screen();
	mvprintw(2, max.x / 2 - 12, "-*_*-Welcome to NEUN-*_*-\n");
	mvprintw(5, 5, "\tCTRL+F:\tHelp\n");
	mvprintw(6, 5, "\tCTRL+E:\tStart Coding...\n");
	mvprintw(7, 5, "\tESC:\tQuit\n");
	refresh();
	return;
}

/* delete_char -> to delete a character on the screen and off the screen.
 */

void delete_char(line *present) {
	delch();
	refresh();
	int i;
	for(i = norm.x; i <= present->chars + 1; i++) {
		present->str[i] = present->str[i + 1];
		refresh();
	}
	move(norm.y, 0);
	clrtoeol();
	mvprintw(norm.y, 0, "%s",present->str);
	move(norm.y, norm.x);
	present->chars--;
	getyx(stdscr,norm.y, norm.x);
	return;
}

/* enter_char -> to enter a character on the screen and off the screen.
 */

void enter_char(line *present, int pos) {
	int i;
	for(i = present->chars + 1; i < pos; i--) {
		present->str[i] = present->str[i - 1];
	}
	move(norm.y, 0);
	clrtoeol();
	printw("%s",present->str);
	move(norm.y, norm.x);
	present->chars--;
	return;
}

/* insert_line -> to enter/insert a line on the screen and off the screen.
 */

line *insert_line(file *fptr) {
	int i;
	line *move = fptr->first;
	line *new;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	new = (line *)malloc(sizeof(line));
	new->str = (char *)malloc(max.x);
	return move;
}

/*new_line -> when initially typing,
 *to get every line entered to a different "line" off the screen.
 */

int new_line(file *fptr, char *str) {
	int i = 0, flag = 0;
	int ch = 1;
	line *first;
	first = fptr->first;
	line *tmp;
	tmp = (line *)malloc(sizeof(line));
	line *move = first;
	if(first == NULL) {
		fptr->first = tmp;
		fptr->first->prev = NULL;
		fptr->first->next = NULL;
		move = fptr->first;
	}
	else {
		while(move->next != NULL)
		{
			move = move->next;
		}
		tmp->prev = move;
		move->next = tmp;
		move = move->next;
		move->next = NULL;
	}
	move->str = (char *)malloc(max.x);
	if(str != NULL) {
		strcpy(move->str, str);
		move->chars = strlen(str);
		return flag;
	}
	while(ch != '\n') {
		move(norm.y, norm.x);
		cbreak();
		ch = getch();
		if(ch == ESC) {
			move(max.y - 1, 0);
			flag = 1;
			break;
		}
		else if(isprint(ch)) {
			printw("%c", ch);
			move->str[i] = ch;
			i++;
			move->chars = i;
			getyx(stdscr, norm.y, norm.x);
		}
		else if(ch == '\n')
			break;
		else {
			move->str[i] = '\0';
			move->chars = i;
			move = other_keys(ch, fptr);
		}
		mvprintw(max.y - 1, max.x - 15 ,"(%d, %d)", norm.y, norm.x);
		move(norm.y, norm.x);
	}
	move->str[i] = '\0';
	move->chars = i;
	return flag;
}

/* other_keys -> to detect the keys other than the printable keys.
 */

line *other_keys(int ch, file *fptr) {
	line *move = fptr->first;
	int i;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	switch(ch) {
		case KEY_DOWN:
			move = key_down(fptr);
			break;
		case KEY_UP:
			move = key_up(fptr);
			break;
		case KEY_RIGHT:
			move = key_right(fptr);
			break;
		case KEY_LEFT:
			move = key_left(fptr);
			break;
		case KEY_BACKSPACE:
			if(norm.x == 0) {
				move = move->prev;
				move(norm.y, norm.x = move->chars);
			}
			else
				move(norm.y, --norm.x);
			delete_char(move);
			break;
		case DELETE:
			delete_char(move);
			break;
	}
	return move;
}	

/*save -> to save the file in the ROM.
 */

void save(file *fptr, char *fname) {
	FILE *fp;
	fp = fopen(fname, "w");
	line *move = fptr->first;
	if(move == NULL)
	{
		attron(A_REVERSE);
		mvprintw(max.y - 1, 0,"No file found");
		attroff(A_REVERSE);
	}
	while(move != NULL) {
		fprintf(fp,"%s", move->str);
		fputc(13, fp);
		move = move->next;
	}
	fseek(fp, -1, SEEK_END);
	return;
}

/* editor -> the main editor of the text editor.
 */

void editor(char *fname) {
	file *fptr = (file *)malloc(sizeof(file));
	fptr->lines = line_no =  0;
	fptr->first = NULL;
	char *str;
	FILE *fp;
	int ch = 1;
	int i = 0, test;
	fp = fopen(fname, "a+");
	if(fp == NULL) {
		printw("Exit\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_SET);
	move(norm.y = 0, norm.x = 0);
	ch = fgetc(fp);
	if(!feof(fp)) {
		fseek(fp, 0, SEEK_SET);
	}
	while(!feof(fp)) {
		str = (char *)malloc(max.x + 1);
		for(i = 0; ch != 13 && !feof(fp); i++) {
			ch = fgetc(fp);
			str[i] = ch;
			printw("%c", ch);
			getyx(stdscr, norm.y, norm.x);
		}
		str[i] = '\0';
		move(norm.y++, norm.x = 0);
		getyx(stdscr, norm.y, norm.x);
		new_line(fptr, str);
		line_no++;
		fptr->lines++;
		ch = 0;
		if(norm.y > max.y - 2) {
			scrl(max.y / 2);
			move(norm.y = (max.y / 2 - 1), 0);
		}
	}
	while(!(test = new_line(fptr, NULL))) {
		if(norm.y > max.y) {
			scrl(max.y / 2);
			move(norm.y = (max.y / 2 - 1), 0);
		}
		line_no++;
		fptr->lines++;
		move(norm.y++, norm.x = 0);
	}
	fclose(fp);
	ch = getch();
	switch (ch) {
		case CTRL_F :
			mvprintw(max.y - 1, 0,"Help");
			help();
			break;
		case CTRL_A:
		case 's':
			mvprintw(max.y - 1, 0,"Saved");
			save(fptr, fname);
			break;
	}
	return;
}

/* set_max -> to get the maximum dimensions of the terminal open.
 */

void set_max() {
	getmaxyx(stdscr, max.y, max.x);
}

/* help -> the help page of the editor
 */

void help() {
	int ch;
	any_screen();
	mvprintw(2, max.x / 2 - 6, "-*_*-NEUN-*_*-\n");
	mvprintw(5, 5, "\tOn the homepage, press CTRL+E to start editing your text.");
	mvprintw(7, 5, "\tWhile editing, after finishing,");
	mvprintw(8, 5, "\t\tpress ESC key to perform operations");
	mvprintw(10, 5, "\tAfter ESC, press 's' or CTRL+A to save your file,");
	mvprintw(12, 5, "\tOr Again press ESC to exit without saving.");
	mvprintw(14, 5, "\tAfter saving, press ESC to exit the NEUN...");
	mvprintw(16, 5, "\t TO EXIT THIS SCREEN PRESS \"ESC\".");
	while(ch != ESC)
		ch = getch();
	return;
}

/*main -> main function of the program.
 */

int main (int argc, char *argv[]) {
	int ch;
	char fname[32];
	int check;
	check = strcmp(argv[1], "-h");
	if(check != 0) {
		check = strcmp(argv[1], "--help");
	}
	if(check == 0) {
		printf("\t\t\t-*_*-NEUN-*_*-\n");
		printf("\tOn the homepage, press CTRL+E to start editing your text.\n");
		printf("\tWhile editing, after finishing,\n");
		printf("\t\tpress ESC key to perform operations.\n");
		printf("\tAfter ESC, press 's' or CTRL+A to save your file,\n");
		printf("\tOr Again press ESC to exit without saving.\n");
		printf("\tAfter saving, press ESC to exit the NEUN...\n\n");
		printf("\tTO EXIT THIS SCREEN PRESS \"ESC\".\n");
		return 0;
	}
	any_screen();
	initscr();
	set_max();
	keypad(stdscr, 1);
	homepage();
	noecho();
	cbreak();
	while(1) {
		ch = getch();
		switch(ch) {
			case CTRL_F:
				move(max.y - 1, 0);
				clrtoeol();
				printw("Help");
				help();
				homepage();
				break;
			case CTRL_E:
				move(max.y - 1, 0);
				clrtoeol();
				printw("Start Editing");
				if(argc == 1) {
					clear();
					attron(A_REVERSE);
					mvprintw(3, 3, "No File Name\n");
					mvprintw(4, 3, "Enter new file name :\n");
					move(5, 3);
					attroff(A_REVERSE);
					echo();
					nocbreak();
					scanw("%s",fname);
					noecho();
					cbreak();
					clear();
					editor(fname);
				}
				else if(argc == 2) {
					clear();
					editor(argv[1]);
				}
				break;
			case ESC:
				refresh();
				endwin();
				return 0;
			default:
				attron(A_REVERSE);
				mvprintw(max.y - 1, 0, "see HELP (CTRL+F) for correct options");
				attroff(A_REVERSE);
		}
	}
	refresh();
	endwin();
	return 0;
}

