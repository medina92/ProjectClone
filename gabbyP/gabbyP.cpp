#include "gabbyP.h"
//Struct button initiation found in header
Button button[MAXBUTTONS];
//--------------START MENU RENDERING----------------------------------------------------
void StartMenu(void)
{
	glPushMatrix();

	//glTranslatef((xres/2), (yres/2) - 200, 0.0f);
	glBindTexture(GL_TEXTURE_2D, StartTexture);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(1.0,1.0); glVertex2f(xres,yres);
	glTexCoord2f(1.0,0.0); glVertex2f(xres,0);
	glTexCoord2f(0.0,1.0); glVertex2f(0,yres);
	glTexCoord2f(0.0,0.0); glVertex2f(0,0);
	glEnd();
	glPopMatrix();

	buttonRender();
	buttonsInit();


}
//-----------------START MENU BUTTON CLICK INITIATED START,LOAD,QUIT---------------------
void mouse_click(int action)
{ 
	if(start)
	{ 
		if (action == 1)
		{
			int i=0;
			//center of a grid

			for (i=0; i<nbuttons; i++) {
				if (button[i].over) {
					button[i].down = 1;
					button[i].click = 1;
					
					if (i==0) {
						//user clicked TOP button (start game)
						start=0;
					}
					if (i==1) {
						//TO BE INITIATED
					}
					if (i==2) {
						//user clicked QUIT
						done=1;
					}
				}
			}
		}
	}
}

//----------------CHECK THE MOUSE/MOUSE CLICK------------------------------------------
void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	int x,y;
	int i;
	lbutton=0;
	rbutton=0;

	if (e->type == ButtonRelease) {
		mouse_click(2);
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	
	//reverse the y position
	y = yres - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	if (x == savex && y == savey)
		return;
	savex=x;
	savey=y;


	for (i=0; i<nbuttons; i++) {
		button[i].over=0;
		button[i].down=0;


		if (x >= button[i].r.left &&
			x <= button[i].r.right &&
			y >= button[i].r.bot &&
			y <= button[i].r.top) {
			button[i].over=1;
			break;
		}
	}
	if (lbutton)
		mouse_click(1);
	if (rbutton)
		mouse_click(2);
}

//------------------------BUTTON RENDERS HERE/HOVER COLOR---------------------------
void buttonRender(void)
{
	int i;
	Rect r;
	for (i=0; i<nbuttons; i++) {
		if (button[i].over) {
		//Highlight Color
		glColor3f(1.0f, 0.0f, 0.0f);
		//draw a highlight around button
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
		glVertex2i(button[i].r.left-2,  button[i].r.top+2);
		glVertex2i(button[i].r.right+2, button[i].r.top+2);
		glVertex2i(button[i].r.right+2, button[i].r.bot-2);
		glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
		glEnd();
		glLineWidth(1);
		}
		if (button[i].down) {
			glColor3fv(button[i].color);
		} else {
			glColor3fv(button[i].color);
		}
		glBegin(GL_QUADS);
		glVertex2i(button[i].r.left,  button[i].r.bot);
		glVertex2i(button[i].r.left,  button[i].r.top);
		glVertex2i(button[i].r.right, button[i].r.top);
		glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();
		r.left = button[i].r.centerx;
		r.bot  = button[i].r.centery-8;
		r.center = 1;
		if (button[i].down) {
			ggprint16(&r, 0, button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, button[i].text_color, button[i].text);
		}
	}
}
//------------------------------BUTTON DRAWN AND COLORED HERE-----------------------------------
void buttonsInit(void)
{	
	//INITIATE BUTTONS
	nbuttons=0;
	//Start Game
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = 160;
	button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Start Game");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	//Load
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = 85;
	button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Load");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	//Quit button
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = 10;
	button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Quit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
}
