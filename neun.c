/*
 *Copyright (c) 2016 KAJAL SANKLECHA
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

#include "neun.h"

position norm, max;
int line_no;

void any_screen() {
	keypad(stdscr, 1);
	clear();
	refresh();
}

line *key_up(file *fptr) {
	if(line_no == 0)
		return fptr->first;
	line_no--;
	line *move;
	int i;
	move = fptr->first;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	if(norm.y == 0) {
		scrl(max.y / 2);
		move(norm.y = (max.y / 2 - 1), 0);
	}
	if(norm.x >= move->chars) {
		move(--norm.y, norm.x = move->chars - 1);
	}
	else
		move(--norm.y,norm.x);
	return move;
}

line *key_down(file *fptr) {
	line_no++;
	line *move;
	move = fptr->first;
	int i;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	if(norm.y >= max.y) {
		scrl(-(max.y / 2));
		move(norm.y = (max.y / 2 - 1), 0);
	}
	if(norm.x >= move->chars) {
		move(++norm.y, norm.x = move->chars - 1);
	}
	else
		move(++norm.y,norm.x);
	return move;
}

line *key_left(file *fptr) {
	line *move;
	int i;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	if(norm.x <= 0)
		return move;
	move(norm.y, --norm.x);
	return move;
}

line *key_right(file *fptr) {
	line *move;
	int i;
	for(i = 0; i < line_no; i++)
		if(!move->next)
			move = move->next;
	if(norm.x >= move->chars - 1)
		return move;
	move(norm.y, ++norm.x);
	return move;
}

void homepage() {
	any_screen();
	mvprintw(2, max.x / 2 - 12, "-*_*-Welcome to NEUN-*_*-\n");
	mvprintw(5, 5, "\tCTRL+H:\tHelp\n");
	mvprintw(6, 5, "\tCTRL+E:\tStart Coding...\n");
	mvprintw(7, 5, "\tESC:\tQuit\n");
	refresh();
	return;
}

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

int new_line(file *fptr, char *str) {
	int total, i = 0, flag = 0;
	int ch = 1;
	line *first;
	first = fptr->first;
	line *tmp;
	tmp = (line *)malloc(sizeof(line));
	line *move = first;
	line_no++;
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
	}
	move->str = (char *)malloc(max . x);
	move->next = NULL;
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
			move = other_keys(ch, fptr);
		}
		mvprintw(max.y - 1, max.x - 15 ,"( %d, %d)", norm.y, norm.x);
		move(norm.y, norm.x);
	}
	move->str[i] = '\0';
	move->chars = i;
	return flag;
}

line *other_keys(int ch, file *fptr) {
	line *move = fptr->first;
	int i = 0;
//	printw("%d\n", line_no);
	refresh();

	for(i = 0; i < line_no; i++) {
		if(move->next)
			move = move->next;
	}
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
	}
	return move;
}	

void save(file *fptr, char *fname) {
	FILE *fp;
	int test = 0;
	fp = fopen(fname, "w");
	line *move = fptr->first;
	if(move == NULL)
	{
		printw("NULL");
	}
	while(move != NULL) {
		fprintf(fp,"%s", move->str);
		move = move->next;
	}
	fseek(fp, -1, SEEK_END);
		fprintf(fp, "\n");
	return;
}

void editor(char *fname) {
	file *fptr = (file *)malloc(sizeof(file));
	fptr->lines = 0;
	fptr->first = NULL;
	char *str;
	FILE *fp;
	line *move;
	int ch = 1;
	int i = 0, flag = 0, k = 0, test;
	fp = fopen(fname, "a+");
	if(fp == NULL) {
		printw("Exit\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_SET);
	move(norm.y = 0, norm.x = 0);
	k = fscanf(fp, "%c", &ch);
	if(k != -1) {
		fseek(fp, 0, SEEK_SET);
	}
	while(k != -1) {
		str = (char *)malloc(max.x + 1);
		for(i = 0; ch != '\n' && k != -1; i++) {
			k = fscanf(fp, "%c", &ch);
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
		ch = '\0';
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
		move(norm.y++, norm.x = 0);
	}
	fclose(fp);
//	menu();
	ch = getch();
	switch (ch) {
		case CTRL_H :
			mvprintw(max.y - 1, 0,"Help");
//			help();
			break;
		case CTRL_A:
		case 's':
			mvprintw(max.y - 1, 0,"Save");
			save(fptr, fname);
			break;
		case CTRL_G:
			mvprintw(max.y - 1, 0,"Find");
//			find();
			break;
	}
	return;
}

void set_max() {
	getmaxyx(stdscr, max.y, max.x);
}

int main (int argc, char *argv[]) {
	any_screen();
	initscr();
	set_max();
	keypad(stdscr, 1);
	homepage();
	int ch;
	char fname[32];
	noecho();
	cbreak();
	while(1) {
		ch = getch();
		switch(ch) {
			case CTRL_H:
				move(max.y - 1, 0);
				clrtoeol();
				printw("Help");
//				help();
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
					printw("%sprint", fname);
					refresh();
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
				mvprintw(max.y - 1, 0, "see HELP (CTRL+H) for correct options");
				attroff(A_REVERSE);
		}
	}
	refresh();
	endwin();
	return 0;
}
