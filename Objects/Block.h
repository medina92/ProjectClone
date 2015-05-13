#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include <fstream>
#include <iostream>
#include "Shape.h"

using namespace std;

class Block : public Shape {

    private:

        bool breakable;

        bool climb;

        bool wall;
        
        bool edge;

    public:

        Block *next;

        Block *prev;

        ~Block() {
            if (next != NULL) delete next;
        }

        void setEdge(bool iEdge) { edge = iEdge; }

        bool getEdge() { return edge; }
       
        void setWall(bool iWall) { wall = iWall; }

        bool getWall() { return wall; }

        void setClimb(bool iClimb) { climb = iClimb; }

        bool getClimb() { return climb; }

        void setBreakable(bool iBreak) { breakable = iBreak; }

        bool getBreakable() { return breakable; }

        Block() {
            next = prev = NULL;
            width = height = 40;
            climb = false;
            breakable = false;
            wall = false;
            edge = false;
        }
};


#endif //_OBSTACLE_H_
