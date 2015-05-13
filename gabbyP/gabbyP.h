#ifndef _gabbyP_H_
#define _gabbyP_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./../Images/ppm.h"
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include<GL/glx.h>
extern "C" {
#include "./../Fonts/fonts.h"
}
#define random(a) (rand()%a)
#define MAXBUTTONS 8
// Variables
extern int start;
extern int lbutton;
extern int rbutton;
extern int nbuttons;
extern int done;
extern int xres, yres;
//OpenGL variables;
extern GLuint StartTexture;
// Prototypes
void StartMenu(void);
void mouse_click(int action);
void check_mouse(XEvent *e);
void buttonRender (void);
void buttonsInit(void);
// Struct
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
	t_button()
	{
		over=false;
	}
} Button;
//Button button[MAXBUTTONS];
#endif

