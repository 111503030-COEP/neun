/* ------NEUN - A Text Editor------
 *Copyright (C) 2016  KAJAL SANKLECHA

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


/*the libraries of C used in the program.
 */
#include <ncurses.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* The alphabets when pressed with the control key.
 */
#define CTRL_A 1
#define CTRL_B 2
#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_N 14
#define CTRL_O 15
#define CTRL_P 16
#define CTRL_R 18
#define CTRL_T 20
#define CTRL_U 21
#define CTRL_V 22
#define CTRL_W 23
#define CTRL_X 24
#define CTRL_Y 25

/*Other keys of the keyboard.
 *the arrow keys are defined in the library - ncurses.h
 */
#define ESC 27
#define DELETE 330
#define BACKSPACE 263
#define PAGE_UP 339
#define PAGE_DOWN 338
#define HOME 262
#define END 360
#define INSERT 331

/* the structure used as a line in the program.
 */
typedef struct line {
	char *str;
	int chars;
	struct line *next, *prev;
} line;

/* the structure used as the whole file in the program.
 */
typedef struct file {
	int lines;
	line *first;
} file;

/* A structure for the position on the screen.
 */
typedef struct position {
	int x, y;
} position;

/* Function prototypes of all the functions in the program.
 */
void any_screen();
line *key_up(file *fptr);
line *key_down(file *fptr);
line *key_left(file *fptr);
line *key_right(file *fptr);
void homepage();
void delete_char(line *present);
void enter_char(line *present, int pos);
line *insert_line(file *fptr);
int new_line(file *fptr, char *str);
line *other_keys(int ch, file *fptr);
void save(file *fptr, char *fname);
void editor(char *fname);
void set_max();
void help();
