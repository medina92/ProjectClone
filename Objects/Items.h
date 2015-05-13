//---------------------------------------------------------
//this is not the only code I have written in this project
//I've worked on game.h, project.cpp,physics.h, and more.
//---------------------------------------------------------
#ifndef _ITEMS_H_
#define _ITEMS_H_
#include <fstream>
#include <iostream>
#include "Shape.h"
#include "../bryantD/Game.h"

using namespace std;
Ppmimage *itemsImage=NULL;
GLuint itemsTexture;

class Items : public Shape 
{
    public:
        Items *next;
        Items *prev;
		bool col;
		char* type;	
		~Items() 
		{
	        if (next != NULL) delete next;
		}
        Items() 
		{
			col = false;
        }
};
class Heart : public Items 
{
    public:
		~Heart() 
		{
			if (next != NULL) delete next;
		}
        Heart() 
		{
			type="heart";
			col = false;
        }
};

class Diamond : public Items 
{
    public:
		~Diamond() 
		{
	        if (next != NULL) delete next;
		}
        Diamond() 
		{
			type="diamond";
			col = false;
        }
};

class Dagger : public Items 
{
    public:
		~Dagger() 
		{
	        if (next != NULL) delete next;
		}
        Dagger() 
		{
			type="dagger";
			width = 14;
			height = 24;
			col = false;
        }
};

class Axe : public Items 
{
    public:
		~Axe() 
		{
	        if (next != NULL) delete next;
		}
        Axe() 
		{
			type="axe";
			width = 14;
			height = 24;
			col = false;
        }
};
class P_Dagger : public Shape 
{
    public:
		int velX;
		int velY;
		P_Dagger *next;
		P_Dagger *prev;
		struct timespec time;
		~P_Dagger() 
		{
	        if (next != NULL) delete next;
		}
        P_Dagger() 
		{
			width = 14;
			height = 24;
        }
		int getVelX() { return velX; }
		int getVelY() { return velY; }
		void setVelX(int vel) { velX = vel; } 
		void setVelY(int vel) { velY = vel; } 
};

class P_Axe : public Shape 
{
    public:
		int velX;
		int velY;
		P_Axe *next;
		P_Axe *prev;
		struct timespec time;
		~P_Axe() 
		{
	        if (next != NULL) delete next;
		}
        P_Axe() 
		{
			width = 14;
			height = 24;
			velY = 10;
			setVelY(10);
        }
		int getVelX() { return velX; }
		int getVelY() { return velY; }
		void setVelX(int vel) { velX = vel; } 
		void setVelY(int vel) { velY = vel; } 
		
};

#endif //_OBSTACLE_H_
