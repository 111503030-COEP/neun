#include <ncurses.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3 // to close the program
#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_H 8 // backspace
#define CTRL_I 9 // tab
#define CTRL_J 10 // enter
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_M 13 // CTRL_J ->	enter
#define CTRL_N 14
#define CTRL_O 15
#define CTRL_P 16
#define CTRL_Q 17 // not used
#define CTRL_R 18 
#define CTRL_S 19 // not used
#define CTRL_T 20
#define CTRL_U 21
#define CTRL_V 22
#define CTRL_W 23
#define CTRL_X 24
#define CTRL_Y 25
#define CTRL_Z 26 // pause the program
#define ESC 27
#define DELETE 330
#define BACKSPACE 263
#define PAGE_UP 339
#define PAGE_DOWN 338
#define HOME 262
#define END 360
#define INSERT 331


typedef struct line {
	char *str;
	int pos, chars;
	struct line *next, *prev;
} line;

typedef struct file {
	int lines;
	line *first;
} file;

typedef struct position {
	int x, y;
} position;

