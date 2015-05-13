//cs335 Spring 2015
//program: project.cpp
//author:  Justin Ramirez
//date:    2015
//
//This program is a game starting point for 335
//

//libraries
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include<typeinfo>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>    
//#include <GL/gl.h>    
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "Images/ppm.h"
//#include "log.h"
//#include "Objects/Items.h"
//#include "Mechanics/Game.h"
#include "bryantD/Game.h"
//#include "Mechanics/Physics.h"
//#include "Mechanics/bryantD.cpp"
#include "bryantD/bryantD.cpp"
//#include "Animation/Animation.h"
#include "danielM.cpp"
#include "justinR.cpp"
#include "UI/UI.h"
//#include "UI/justinR.h"
#include <ctype.h>
#include "gustavoC/gustavoC.h"
#include "gabbyP/gabbyP.h"
extern "C" {
#include "Fonts/fonts.h"
}
//-----------------------------------------------------------------------------

//macros, not used
/*
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
(c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
 */
//-----------------------------------------------------------------------------

//constants
//const float timeslice = 1.0f;
#define PI 3.141592653589793
#define ALPHA 1
//-----------------------------------------------------------------------------

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;
//-----------------------------------------------------------------------------

//Setup timers,                             time not used
const double physicsRate = 1.0 / 60.0;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;


//-----------------------------------------------------------------------------

//Global variables
int xres=1250, yres=900;
#define KEY_MAX 65536
int keys[KEY_MAX];
//-----------------------------------------------------------------------------

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
//void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void init_sounds(void);
void init(Game *g);
void physics(Game *game);
void render(Game *game);
void UIRender(Game *game);
//Start Image Var
Ppmimage *startImage=NULL;
//Start Texture
GLuint StartTexture;
//Start Button Init
int done=0;
int lbutton = 0;
int rbutton = 0;
int nbuttons= 0;
int start = 1;

//-----------------------------------------------------------------------------

int main(void)
{
	//logOpen();
	initXWindows();
	init_opengl();
	Game game;
	if(!fmod_init()) return -1;
	init(&game);
	initTimers(&game);
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	game.setRun(true);
	while (!done) {
		XEvent e;
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics(&game);
			physicsCountdown -= physicsRate;
		}
		render(&game);
		UIRender(&game);
		game.setRun(false);
		glXSwapBuffers(dpy, win);

		// must update fmod often
		fmod_systemupdate();
	}
	cleanupXWindows();
	fmod_cleanup();
	cleanup_fonts();
	//logClose();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "CS335 - Project template");
}

void setup_screen_res(const int w, const int h)
{
	xres = w;
	yres = h;
}

void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XSetWindowAttributes swa;
	setup_screen_res(xres, yres);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
        //MOUSE INITIATION DETECT MOVEMENT
        swa.event_mask = ExposureMask |
			 KeyPressMask |
			 KeyReleaseMask |
			 ButtonPressMask |
			 ButtonReleaseMask |
			 PointerMotionMask |
			 StructureNotifyMask |
			 SubstructureNotifyMask; 
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	//XAutoRepeatOff(dpy);
}

void reshape_window(int width, int height)
{
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	set_title();
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//init images
	simonImage = ppm6GetImage("./Images/simon-belmont-2.ppm");
	blockImage = ppm6GetImage("./Images/block1.ppm");
	uiImage = ppm6GetImage("./Images/ui.ppm");
	itemsImage = ppm6GetImage("./Images/items.ppm");
	backImage = ppm6GetImage("./Images/background.ppm");
	startImage = ppm6GetImage("./Images/start.ppm");
	//Set pixel red colors to black 
	setColorBlack(simonImage);
	//creating openGl texture elements
	glGenTextures(1, &simonTexture);
	glGenTextures(1, &silhouetteTexture);
        glGenTextures(1, &StartTexture);
	//-------------------------------------------------------------------
	//Simon
	//
	int w = simonImage->width;
	int h = simonImage->height;
	//
	glBindTexture(GL_TEXTURE_2D, simonTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, simonImage->data);
	//-------------------------------------------------------------------
	//
	//silhouette
	//displays sprite to screen 
	//
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *silhouetteData = buildAlphaData(simonImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	//-------------------------------------------------------------------
	//init UI
	//Ppmimage *uiImage=NULL;
	//GLuint uiTexture;
	//Set pixel red colors to black 
	setColorBlack(uiImage);
	//creating openGl texture elements
	glGenTextures(1, &uiTexture);
	glGenTextures(1, &uiTexture);
	//-------------------------------------------------------------------
	w = uiImage->width;
	h = uiImage->height;
	//
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, uiImage->data);
	//-------------------------------------------------------------------
	//displays sprite to screen 
	//
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *uiData = buildAlphaData(uiImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, uiData);
	delete [] uiData;
	//-------------------------------------------------------------------

	glGenTextures(1, &blockTexture);
	w = blockImage->width;
	h = blockImage->height;

	glBindTexture(GL_TEXTURE_2D, blockTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, blockImage->data);


	glGenTextures(1, &backTexture);
	w = backImage->width;
	h = backImage->height;

	glBindTexture(GL_TEXTURE_2D, backTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, backImage->data);
	////////////////////////////items
	//Set pixel red colors to black 
	setColorBlack(itemsImage);
	//creating openGl texture elements
	glGenTextures(1, &itemsTexture);
	//glGenTextures(1, &itemsTexture);
	//-------------------------------------------------------------------
	w = itemsImage->width;
	h = itemsImage->height;
	//
	glBindTexture(GL_TEXTURE_2D, itemsTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, itemsImage->data);
	//-------------------------------------------------------------------
	//displays sprite to screen 
	//
	glBindTexture(GL_TEXTURE_2D, itemsTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *itemsData = buildAlphaData(itemsImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, itemsData);
	//  delete [] itemsData;
	//START SCREEN------------------------------------------
    	glBindTexture(GL_TEXTURE_2D, StartTexture);
    	
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    	glTexImage2D(GL_TEXTURE_2D, 0, 3,
            startImage->width, startImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, startImage->data);
    	//-------------------------------------------------------
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
		xres = xce.width;
		yres = xce.height;
	}
}

void init(Game *g) {
	memset(keys, 0, 65536);
	//set some initial parameters
	int row = 20;
	int col = 70;
	char **map;
	char *lvl = "Levels/lvl1.txt";
	map = g->getLevel(lvl,row,col);
	Block *b;
	Items *it;
	Ghouls *gs;

	for (int r = 0; r < row; r++){
		for(int c = 0; c < col; c++){
			if (!isspace(map[r][c])){
				if (map[r][c] == 'S') 
					g->player.setPos(((2*40*c)+40), (900-(2*40*r-35))); 
				if (map[r][c] == 'h'){
					it = g->createHeart();
					it->setCenter(((2*40*c)+40), (yres-(2*40*r)-35)); 
					continue;
				}
				if (map[r][c] == 'd'){
					it = g->createDiamond();
					it->setCenter(((2*40*c)+40), (yres-(2*40*r)-35)); 
					continue;
				}
				if (map[r][c] == 't'){
					it = g->createDagger();
					it->setCenter(((2*40*c)+40), (yres-(2*40*r)-35)); 
					continue;
				}
				if (map[r][c] == 'T'){
					it = g->createAxe();
					it->setCenter(((2*40*c)+40), (yres-(2*40*r)-35)); 
					continue;
				}
				if (map[r][c] == 'G') {
					gs = g->createGhoul();
					gs->setPos(((2*40*c)+40), (900-(2*40*r-35))); 
				}

				b = g->createBlock();
				b->setCenter(((2*40*c)+40), (900-(2*40*r)-40));  

				if (map[r][c] == 'C') { 
					b->setClimb(true); 
					b->setHeight(10);
				}

				if (map[r][c] == 'w')  
					b->setWall(true); 

				if (map[r][c] == 'e')  
					b->setEdge(true); 
			}
		}

	}
	P_Axe * ax = g->axeHead;
	while (ax) {
		ax->setVelY(10);
		ax = ax->next;
	}

	// change this if you want, this was here for testing
	fmod_createsound("./Sounds/bgm2.flac",BGM);
	// setting to loop for now (it is bgm atm)
	fmod_setmode(BGM,FMOD_LOOP_NORMAL);
	// play sound; bool does nothing atm
	fmod_playsound(BGM);
}

int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = XLookupKeysym(&e->xkey, 0);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		// hopefully fix segfaults
		if(key >= 0 && key <= KEY_MAX)
			keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			// release shift (unused?) and dash
			shift=0;
			dash=0;
		}
		return 0;
	}
	else if (e->type == KeyPress) {
		// hopefully fix segfaults
		if(key >= 0 && key <= KEY_MAX)
			keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			// set shift (unused?) and dash
			shift=1;
			dash=1;
			return 0;
		}
		if(key == XK_l) {
			attack = 1;
		}
	} else {
		return 0;
	}
	// if (shift){}  not used
	switch(key) {
		case XK_Escape:
			done=1;
			/*  case XK_f:
				break;
				case XK_s:
				break;
				case XK_Down:
				break;
				case XK_equal:
				break;
				case XK_minus:
				break;*/
	}
	return 0;
}

void physics(Game *g)
{	
	Ghouls *gh = g->ghoulHead;
	playerFwd(g);	
	playerJump(g);
	playerForces(g);
	characterCollision(g);
	while(gh) {
		ghoulForces(gh);
		ghoulCollision(g, gh);
		hitDetection(g, gh);
		ghoulMovement(gh);
		gh = gh->next;
	}
	//update projectile positions
	updateActive(g, keys);
	updateDaggers(g, keys);
	updateAxes(g, keys);
	checkItemCollision(g);
	applyKey(g, keys);
}

void render(Game *g)
{	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
    if(start) {
	StartMenu();
	}
	if(!start)
    {
	backGround();
    
	Block *bh = g->blockHead;
	Items *it = g->itemHead;
	Ghouls *gh = g->ghoulHead;
	P_Dagger *dg = g->dagHead;
	P_Axe *ax = g->axeHead;
	int blockWidth = bh->getWidth();
	int blockHeight = bh->getHeight();
	float blockX;
	float blockY;
	float ghoulW = gh->getWidth();
	float ghoulH = gh->getHeight();

	glPushMatrix();

	float dist = g->player.getPosX()-(xres/2);
	float dist2 = g->player.getPosX()+(xres/2);

	if (dist < 0) {
		glTranslatef(0.0,-g->player.getPosY()+200, 0.0f);
	}
	else if (dist2 > 5600) {
		glTranslatef(-4200.0,-g->player.getPosY()+200, 0.0f);
	}
	else {
		glTranslatef(-g->player.getPosX()+xres/2,-g->player.getPosY()+200, 0.0f);
	}

	while(bh) {
		blockSprite(bh);
		bh = bh->next;
	}

	while(gh) {
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		glTranslatef(gh->getPosX(), gh->getPosY(), 0.0f);
		glBegin(GL_QUADS);

		glVertex2f(-ghoulW, -ghoulH);
		glVertex2f(-ghoulW, ghoulH);
		glVertex2f(ghoulW, ghoulH);
		glVertex2f(ghoulW, -ghoulH);
		glEnd();
		glPopMatrix();
		gh = gh->next;
	}
	while(ax) {
		renderAxe(g, ax);
		ax = ax->next;
	}
	while(dg) {
		renderDagger(g, dg);
		dg = dg->next;
	}
	while(it) {
		renderItem(g, it);
		it=it->next;
	}

	/*for (int i = 0; i < g->nGhouls; i++) {
	//ghoulSprite(gh);
	gh = gh->next;
	}*/

	spriteAnimation(g, keys, physicsRate, timeSpan);
	glPopMatrix();
	}
  }    
void UIRender(Game *g){
    if(!start)
    {
	renderHealth(g,xres,yres);
	renderScore(g,xres,yres);
	renderItemSlot(g,xres,yres);
    }
}
