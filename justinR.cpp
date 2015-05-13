//Author: Justin Ramirez
//Spring 2015
//This is where the code for:
//	 updating the projectiles 
//   rendering the projectiles
//   checking for item collisions
//resides.
//This is not the only code I have written,
//I have contributed to almost every file.
#include "bryantD/Game.h"
#include <ctime>

using namespace std;
const double oobillion = 1.0 / 1e9;

double timeDiff(struct timespec *start, struct timespec *end) 
{
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source) 
{
	memcpy(dest, source, sizeof(struct timespec));
}

struct timespec t2;

void initTimers(Game *g) 
{
	clock_gettime(CLOCK_REALTIME, &g->DagTimer);
	clock_gettime(CLOCK_REALTIME, &g->axeTimer);
	clock_gettime(CLOCK_REALTIME, &t2);

}
void updateActive(Game *g, int *keys)
{
	if (keys['c']) {
		struct timespec t1;
		clock_gettime(CLOCK_REALTIME, &t1);
		//clock_gettime(CLOCK_REALTIME, &t2);
		double ts = timeDiff(&t2,&t1);
		if (ts > 0.2)
			g->player.switchActive();
		clock_gettime(CLOCK_REALTIME, &t2);
	} 

}
void updateDaggers(Game *g, int *keys)
{
	if (g->player.daggers == 0){
		g->player.hasDaggers = false;
		if (g->player.hasAxes)
			g->player.setAxeActive();
	}
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	P_Dagger *dg = g->dagHead;

	while(dg) {
		double ts = timeDiff(&dg->time, &bt);

		if (ts > 0.5) {
			P_Dagger *savedg = dg->next;
			g->deleteDagger(dg);
			g->player.daggers--;
			dg = savedg;
			continue;
		}
		dg->setCenterX(dg->getCenterX() + dg->getVelX());// b->vel[0];
		dg = dg->next;
	}
	if (g->player.hasDaggers && g->player.dagActive) {
		if (keys['x']) { 
			struct timespec bt; 
			clock_gettime(CLOCK_REALTIME, &bt); 
			double ts = timeDiff(&g->DagTimer, &bt); 

			if ( ts > 0.5) { 
				timeCopy(&g->DagTimer, &bt); 
				//shoot a daggie... 
				P_Dagger *dg = (P_Dagger *)malloc(sizeof(P_Dagger)); 
				timeCopy(&dg->time, &bt); 
				dg->setCenterX(g->player.getPosX()); 
				dg->setCenterY(g->player.getPosY()); 

				if (g->player.getFwd())
					dg->setVelX(g->player.getVelX()+20);
				else
					dg->setVelX(g->player.getVelX()-20);
				//put daggie into linked list 
				dg->prev = NULL; 
				dg->next = g->dagHead; 
				if (g->dagHead != NULL) 
					g->dagHead->prev = dg; 
				g->dagHead = dg; 
			} 
		} 
	}
}

void updateAxes(Game *g, int *keys){
	if (g->player.axes == 0){
		g->player.hasAxes = false;
		if (g->player.hasDaggers)
			g->player.setDagActive();
	}
	//update axe positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	P_Axe *ax = g->axeHead;
	while(ax) {
		double ts = timeDiff(&ax->time, &bt);
		if (ts > 0.5) {
			P_Axe *saveax = ax->next;
			g->deleteAxe(ax);
			g->player.axes--; 
			ax = saveax;
			continue;
		}
		ax->setCenterX(ax->getCenterX() + ax->getVelX());// b->vel[0];
		//ax->setVelY(10);
		ax->setCenterY(ax->getCenterY() + ax->getVelY());// b->vel[0];
		ax->setVelY(ax->getVelY()-1.2);
		ax = ax->next;
		//g->player.axes = g->player.axes - 1;
	}
	if (g->player.axes > 0 && g->player.axeActive){
		if (keys['x']) { 
			//		struct timespec bt; 
			clock_gettime(CLOCK_REALTIME, &bt); 
			double ts = timeDiff(&g->axeTimer, &bt); 

			if ( ts > 0.5) { 
				timeCopy(&g->axeTimer, &bt); 
				//shoot an axe... 
				P_Axe *ax = (P_Axe *)malloc(sizeof(P_Axe)); 
				timeCopy(&ax->time, &bt); 
				ax->setCenterX(g->player.getPosX()); 
				ax->setCenterY(g->player.getPosY()); 
				ax->setVelY(17); 

				if(g->player.getFwd())
					ax->setVelX(g->player.getVelX()+15);
				else
					ax->setVelX(g->player.getVelX()-15);

				//put daggie into linked list 
				ax->prev = NULL; 
				ax->next = g->axeHead; 

				if (g->axeHead != NULL) 
					g->axeHead->prev = ax; 
				g->axeHead = ax;
			} 
		} 
	}
}

void renderAxe(Game *g, P_Axe *ax)
{
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glTranslatef(ax->getCenterX(), ax->getCenterY(), 0.0f);

	if (!g->player.getFwd())
		glRotatef(45.0f,0.0f,0.0f,1.0f);
	else
		glRotatef(-45.0f,0.0f,0.0f,1.0f);

	ax->setWidth(14);    
	ax->setHeight(24);    
	glEnable(GL_ALPHA_TEST); 
	glAlphaFunc(GL_GREATER, 0.0f); 
	glColor4ub(225,225,225,225); 
	glBindTexture(GL_TEXTURE_2D, itemsTexture); 
	glBegin(GL_QUADS);
	glTexCoord2f(0.30f,0.38f);glVertex2f(-ax->getWidth(), -ax->getHeight());
	glTexCoord2f(0.30f,0.0f);glVertex2f(-ax->getWidth(), ax->getHeight());
	glTexCoord2f(0.49f,0.0f);glVertex2f(ax->getWidth(), ax->getHeight());
	glTexCoord2f(0.48f,0.38f);glVertex2f(ax->getWidth(), -ax->getHeight());
	glEnd();
	glPopMatrix();
}

void renderDagger(Game *g, P_Dagger *dg)
{
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glTranslatef(dg->getCenterX(), dg->getCenterY(), 0.0f);

	if (!g->player.getFwd())
		glRotatef(90.0f,0.0f,0.0f,1.0f);
	else
		glRotatef(270.0f,0.0f,0.0f,1.0f);

	dg->setWidth(14);    
	dg->setHeight(24);    
	glEnable(GL_ALPHA_TEST); 
	glAlphaFunc(GL_GREATER, 0.0f); 
	glColor4ub(225,225,225,225); 
	glBindTexture(GL_TEXTURE_2D, itemsTexture); 
	glBegin(GL_QUADS);
	glTexCoord2f(0.11f,0.24f);glVertex2f(-dg->getWidth(), -dg->getHeight());
	glTexCoord2f(0.11f,0.0f);glVertex2f(-dg->getWidth(), dg->getHeight());
	glTexCoord2f(0.26f,0.0f);glVertex2f(dg->getWidth(), dg->getHeight());
	glTexCoord2f(0.26f,0.24f);glVertex2f(dg->getWidth(), -dg->getHeight());
	glEnd();
	glPopMatrix();
}
void renderItem(Game *g, Items *it)
{
	int itW, itH; 
	itW = 10;
	itH = 10;

	if(it->type == "heart") {
		glPushMatrix();
		glTranslatef(it->getCenterX(), it->getCenterY(), 0.0f);
		glEnable(GL_ALPHA_TEST); 
		glAlphaFunc(GL_GREATER, 0.0f); 
		glColor4ub(225,225,225,225); 
		glBindTexture(GL_TEXTURE_2D, itemsTexture); 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.1f);glVertex2f(-itW, -itH);
		glTexCoord2f(0.0f,0.0f);glVertex2f(-itW, itH);
		glTexCoord2f(0.1f,0.0f);glVertex2f(itW, itH);
		glTexCoord2f(0.1f,0.1f);glVertex2f(itW, -itH);
		glEnd();
		glPopMatrix();
	} else if (it->type == "diamond") {
		glPushMatrix();
		glTranslatef(it->getCenterX(), it->getCenterY(), 0.0f);
		glEnable(GL_ALPHA_TEST); 
		glAlphaFunc(GL_GREATER, 0.0f); 
		glColor4ub(225,225,225,225); 
		glBindTexture(GL_TEXTURE_2D, itemsTexture); 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.2f);glVertex2f(-itW, -itH);
		glTexCoord2f(0.0f,0.1f);glVertex2f(-itW, itH);
		glTexCoord2f(0.1f,0.1f);glVertex2f(itW, itH);
		glTexCoord2f(0.1f,0.2f);glVertex2f(itW, -itH);
		glEnd();
		glPopMatrix();
	} else if (it->type == "dagger") {
		glPushMatrix();
		glTranslatef(it->getCenterX(), it->getCenterY(), 0.0f);
		glEnable(GL_ALPHA_TEST); 
		glAlphaFunc(GL_GREATER, 0.0f); 
		glColor4ub(225,225,225,225); 
		glBindTexture(GL_TEXTURE_2D, itemsTexture); 
		glBegin(GL_QUADS);
		glTexCoord2f(0.11f,0.24f);glVertex2f(-it->getWidth(), -it->getHeight());
		glTexCoord2f(0.11f,0.0f);glVertex2f(-it->getWidth(), it->getHeight());
		glTexCoord2f(0.26f,0.0f);glVertex2f(it->getWidth(), it->getHeight());
		glTexCoord2f(0.26f,0.24f);glVertex2f(it->getWidth(), -it->getHeight());
		glEnd();
		glPopMatrix();
	}else if (it->type == "axe") {
		glPushMatrix();
		glTranslatef(it->getCenterX(), it->getCenterY(), 0.0f);
		glEnable(GL_ALPHA_TEST); 
		glAlphaFunc(GL_GREATER, 0.0f); 
		glColor4ub(225,225,225,225); 
		glBindTexture(GL_TEXTURE_2D, itemsTexture); 
		glBegin(GL_QUADS);
		glTexCoord2f(0.30f,0.38f);glVertex2f(-it->getWidth(), -it->getHeight());
		glTexCoord2f(0.30f,0.0f);glVertex2f(-it->getWidth(), it->getHeight());
		glTexCoord2f(0.49f,0.0f);glVertex2f(it->getWidth(), it->getHeight());
		glTexCoord2f(0.48f,0.38f);glVertex2f(it->getWidth(), -it->getHeight());
		glEnd();
		glPopMatrix();
	}
}

void checkItemCollision(Game * g)
{
	Items *item = g->itemHead;

	while(item!=NULL) {
		int itemY = item->getCenterY();
		int itemX = item->getCenterX();
		int itemH = item->getHeight();
		int itemW = item->getWidth();
		//top
		if ( (g->player.getPosX() + g->player.getWidth() > itemX
					&& g->player.getPosX() -g->player.getWidth() < itemX )
				&& (g->player.getPosY() - g->player.getHeight() <= itemY )
				&& (g->player.getPosY() + g->player.getHeight() >= itemY) ) 
		{
			if (item->type == "diamond") {
				g->player.addToScore(500);
			} else if (item->type == "heart") {
				g->player.addToScore(200);
				g->player.incHealth();
			}
			if (item->type == "dagger"){
				g->player.setDagActive();
				g->player.hasDaggers = true;
				g->player.daggers =+ 7;
			}
			if (item->type == "axe"){
				g->player.setAxeActive();
				g->player.hasAxes = true;
				g->player.axes =+ 7;
			}
			Items *saveItem = item;
			g->deleteItem(item);
			item = saveItem;
		}
		item = item->next;
	}
}
