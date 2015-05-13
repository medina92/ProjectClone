//#pragma once
#ifndef _GHOUL_H_
#define _GHOUL_H_

#include <stddef.h>
#include "Character.h"
//Here is where we will add more features to the ghouls. 
//Suggestion, this class can be generalized even more to an enemy class that other enemy types can inherit from.
//Try adding projectile particles, vomit projectile particles that does damage.    


class Ghouls : public Character {

    public: 

        Ghouls *next;        //Ghouls will be formed in a doubley linked list and will be created and deleted that way.
        Ghouls *prev;
        Ghouls() {           //initialze pointers to NULL.
            next = prev = NULL;
            width = 30;
            height = 50;
            col = 0;
            vel[0] = 0;
            vel[1] = 0;
        }

        ~Ghouls() {
            if (next != NULL) delete next;
        }

};

#endif //_GAME_H_
