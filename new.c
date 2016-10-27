#include "neun.h"

position norm, max;
int line_no;
void any_screen() {
	keypad(stdscr, 1);
	clear();
	refresh();
}

void key_up(line *initial) {
	line *new;
	new = initial->prev;
	if(new == NULL)
		return;
	if(norm.x >= new->chars) {
		move(--norm.y, norm.x = new->chars - 1);
	}
	else {
		move(--norm.y,norm.x);
	}
	line_no--;
	return;
}

void key_down(line *initial) {
	line *new;
	new = initial->next;
	if(new == NULL)
		return;
	if(norm.x >= new->chars) {
		move(++norm.y, norm.x = new->chars - 1);
	}
	else {
		move(++norm.y,norm.x);
	}
	line_no++;
	return;
}

void key_left() {
	if(norm.x <= 0)
		return;
	move(norm.y, --norm.x);
	return;
}

void key_right(line *l) {
	if(norm.x >= l->chars - 1)
		return;
	move(norm.y, ++norm.x);
	return;
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
		mvprintw(6, i, "%d \t%c",i, present->str[i]);
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
void insert_line(file *fptr) {
	int i;
	line *move = fptr->first;
	for(i = 0; i < line_no; i++)
		move = move->next;
}

int new_line(int pos, file *fptr, char *str) {
	int total, i = 0, flag = 0;
	int ch = 1;
	line *first;
	first = fptr->first;
	line *tmp;
	tmp = (line *)malloc(sizeof(line));
	line *move = first;
	if(first == NULL) {
		fptr->first = tmp;
		fptr->first->pos = 1;
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
	move->pos = pos;
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
		else {
			switch(ch) {
				case KEY_DOWN:
					key_down(move);
					break;
				case KEY_UP:
					key_up(move);
					break;
				case KEY_RIGHT:
					key_right(move);
					break;
				case KEY_LEFT:
					key_left();
					break;
				case KEY_BACKSPACE:
					move(norm.y, --norm.x);
					delete_char(move);
					break;
			}
	
		}
		mvprintw(max.y - 1, max.x - 15 ,"( %d, %d)", norm.y, norm.x);
		move(norm.y, norm.x);
	}
	move->str[i] = '\0';
	move->chars = i;
	return flag;
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
	return;
}

void editor(char *fname) {
	file *fptr = (file *)malloc(sizeof(file));
	fptr->lines = 0;
	fptr->first = NULL;
	char *str;
	FILE *fp;
	char ch = 1;
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
		move(norm.y++, norm.x = 0);
		str[i] = '\0';
		getyx(stdscr, norm.y, norm.x);
		new_line(fptr->lines, fptr, str);
		line_no++;
		fptr->lines++;
		ch = '\0';
	}
	while(!(test = new_line(fptr->lines, fptr, NULL))) {
		move(norm.y++, norm.x = 0);
	}
	fclose(fp);
//	menu();
/*	switch(test) {
		case KEY_DOWN:
			key_down();
			break;
		case KEY_UP:
			key_up();
			break;
		case KEY_RIGHT:
			key_right();
			break;
		case KEY_LEFT:
			key_left();
			break;
		case KEY_BACKSPACE:
			move(norm.y, --norm.x);
			delete_char();
			move(norm.y, ++norm.x);
			break;
	}
*/	ch = getch();
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

void set_max () {
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
			case CTRL_Q:
			case 's':
				move(max.y - 1, 0);
				clrtoeol();
				printw("Save");
//				save();
				break;
			case CTRL_G:
				move(max.y - 1, 0);
				clrtoeol();
				printw("Find");
//				find();
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
			case 'q':
				refresh();
				endwin();
				return 0;
/*			case KEY_DELETE:
				move(max.y - 1, 0);
				clrtoeol();
				printw("delete");
				break;
*/			default:
				attron(A_REVERSE);
				mvprintw(max.y - 1, 0, "see HELP (CTRL+H) for correct options");
				attroff(A_REVERSE);
		}
	}
	refresh();
	endwin();
	return 0;
}

