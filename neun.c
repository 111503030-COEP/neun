#include <ncurses.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#define CTRL_H 8
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_M 13
#define CTRL_S 19
#define CTRL_G 7
#define ESC 27
#define BackSpace 8
//(vert, hori)
/*
void ctrl_commands(fp) {
	char ch;
	ch = getch();
	
}
*/
void homepage(int, int);
void any_screen() {
	keypad(stdscr, 1);
	clear();
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(max_y - 1, max_x - 25, "width:%d\tHeight:%d",max_x,max_y);
	refresh();
}
int help() {
	any_screen();
	//print whole menu for the commands
	//ctrl + s -> save
	//ctrl + g -> find
	//ctrl + u -> undo
	//ctrl + r -> redo
	return 0;
}
int save(char *fname, char *ftmp) {
	char ch;
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	any_screen();
	if(!(strcmp (fname, ftmp))) {
		mvprintw (max_y - 1, 0, "Enter new file name : ");
		scanw ("%s", fname);
	}
	FILE *fp;
	fp = fopen (fname, "w");
	while(fscanf(fp, "%c", &ch) != -1) {
		fputc(ch, fp);
	}
	return 1;
}
//void cut(char *str) {
	
/*int mark(int init_y, int init_x) {
	char ch;
	char tmp_str[1024];
	int i = 0;
	while(ch != CTRL_M) {
		attron(A_REVERSE);
		ch = 
		
		attroff(A_REVERSE);
	}
	
}*/
int editor(char *fname) {
	initscr();
	any_screen();
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	int y, x;
	stack s;
	init(&s);
	FILE *fp, *fptmp;
	int check_same, sv = 0;
	int ch = 1;
	fp = fopen (fname, "a+");
	if(fp == NULL) {
		printw("File not found\n");
	}
	check_same = strcmp (fname, "_tmp.txt");
	if(check_same != 0) {
		fptmp = fopen ("_tmp.txt", "a+");
		while(!feof(fp)) {
		fscanf(fp,"%c", &ch);
			fprintf(fptmp, "%c", ch);
		}
		fclose(fp);
		fp = fptmp;
		move(0, 0);
		fseek(fp, 0, SEEK_SET);
		while(fscanf (fp,"%c", &ch) != -1) {
			printw("%c", ch);
		}
	}
	else
		move(0, 0);
	getyx(stdscr, y, x);
	move(y, x);
	ch = 1;
	while(1) {
		move(y, x);
		cbreak();
		ch = getch();
		if(ch == ESC)
			break;
		getyx(stdscr, y, x);
		mvprintw(max_y - 1, 0 ,"Current position = ( %d, %d)", y, x);
		move(y, x);
		if(isprint(ch) && ch != '\n') {
			printw("%c", ch);
			move(y, x++);
			fprintf(fp, "%c", ch);
		}
		if(ch == '\n')	{
			push(&s, x);
			move (y++, x = 0);
			fprintf(fp, "%c", ch);
		}
		if(ch == KEY_BACKSPACE) {
			if(x ==	0) {
				move(--y, x = pop(&s));
			}
			else {
				move(y, --x);
				delch();
			}
		}
		
	}
	move(max_y - 1, 0);
	if(ch == ESC) {
//		func_commands();
		ch = getch();
		mvprintw(max_y - 2, 0, "Char recieved is : %d in ascii", ch);
		switch(ch) {
			case CTRL_S:
				mvprintw(max_y - 1, 0,"Save");
				sv = save (fname, "_tmp.txt");
				break;
			default :
				mvprintw(max_y - 1, 0,"Default");
		}
	}
	clear();
	
	if(sv)
		mvprintw(max_y - 1, 0, "File saved");
	refresh();
	endwin();
	fclose(fp);
//	remove("_tmp.txt");
	return 0;
}
int main (int argc, char *argv[]) {
	any_screen();
	int max_y, max_x;
	initscr();
	keypad(stdscr, 1);
	getmaxyx(stdscr, max_y, max_x);
	homepage(max_y, max_x);
	int ch;
	noecho();
	ch = getch();
	switch(ch) {
		case CTRL_H:
			mvprintw(max_y - 1, 0,"Help");
	//		help();
			break;
		case CTRL_S:
			mvprintw(max_y - 1, 0,"Save");
	//		save();
			break;
		case CTRL_G:
			mvprintw(max_y - 1, 0,"Find");
	//		find();
			break;
		case CTRL_E:
			mvprintw(max_y - 1, 0,"Start Editing");
			if(argc == 2)
				ch = editor (argv[1]);
			else if(argc == 1)
				ch = editor ("_tmp.txt");
			break;
		case CTRL_F:
			mvprintw(max_y - 1, 0,"File");
	//		file();
			break;
	}
	refresh();
	getch();
	endwin();
	return 0;
}
void homepage(int max_y,int max_x) {
//	initscr();
	any_screen();
	mvprintw(2, max_x / 2 - 12, "-*_*-Welcome to NEUN-*_*-\n");
	mvprintw(4, 5, "\tCTRL+F:\tFile menu\n");
	mvprintw(5, 5, "\tCTRL+H:\tHelp\n");
	mvprintw(6, 5, "\tCTRL+E:\tStart Coding...\n");
	mvprintw(7, 5, "\tCTRL+C:\tExit\n");
	mvprintw(max_y - 1, max_x - 28, "width:%d\tHeight:%d",max_x,max_y);
	refresh();
	return;
}










/*
	refresh();
	getch();
	endwin();
	initscr();
	clear();
	echo();
	while(ch != 'q') {
		printw("press any key : ");
		scanw("%c", &ch);
		printw("%d", ch);
	}
	mvprintw(max_y - 1, 0,"Trial to clear the screen");
	


		case KEY_DOWN:
			mvprintw(max_y - 1, 0,"Down arrow key pressed");
			break;
		case KEY_UP:
			mvprintw(max_y - 1, 0,"Up arrow key pressed");
			break;
		case KEY_RIGHT:
			mvprintw(max_y - 1, 0,"Right arrow key pressed");
			break;
		case KEY_LEFT:
			mvprintw(max_y - 1, 0,"Left arrow key pressed");
			break;
*/
