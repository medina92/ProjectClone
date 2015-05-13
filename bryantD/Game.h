#ifndef _GAME_H_
#define _GAME_H_

#include <fstream>
#include <iostream>
#include "../Characters/Player.h"
#include "../Characters/Ghoul.h"
#include "../Objects/Whip.h"
#include "../Objects/Block.h"
#include "../Objects/Items.h"
//#include "../Objects/justinRItems.h"

using namespace std;

//typedef float Matrix[4][4];       not used for now

class Game {

    protected:
        bool firstrun;          //first startup
        bool firstrender;       //first time rendering

        bool hit;               //hit enemy or object, true or false
        bool col;               //collision with platform or floor, true of false

    public:
        
        Block *blockHead;       //pointer to begninning of Block linked list
        int nBlocks;            //the number of blocks in the list

        Ghouls *ghoulHead;      //pointer to beginning of Ghoul linked list
        int nGhouls;            //the number of ghouls in the list

        Players player;         //the player, could possibly be a pointer if we want to implement multiplayer

		Items *itemHead;
		int nItems;

		P_Dagger *dagHead;
		int nDags;
		struct timespec DagTimer;
		
		P_Axe *axeHead;
		int nAxes;
		struct timespec axeTimer;

        Game() {
            col = false;        //collision flag, fixes multiple jumps
            firstrun = true;    //first time running game
            firstrender =true;  //first time rendering
            ghoulHead = NULL;   //initialize ghoul head of list to NULL
			itemHead =NULL;
            blockHead = NULL;   //initialize block head of list to NULL
            dagHead = NULL;   //initialize Dagger head of list to NULL
			axeHead = NULL;   //initialize  Axe head of list to NULL
            nGhouls = 0;
            nDags = 0;
            nAxes = 0;
			nItems = 0;
        }

        ~Game() {
        delete blockHead;
        delete ghoulHead;
        delete itemHead;
        }

        bool getHit() { return hit; }

        void setHit(bool iHit) { hit = iHit; }

        bool getCol() { return col; }

        void setCol(bool iCol) { col = iCol; }

        bool getRun() { return firstrun; }

        void setRun(bool iRun) { firstrun = iRun; }

        Ghouls *createGhoul() {
            Ghouls *gh = new Ghouls;
            gh->next = ghoulHead;
            if (ghoulHead != NULL)
                ghoulHead->prev = gh;
            ghoulHead = gh;
            nGhouls++;
            return gh;
        }

        void deleteGhoul(Ghouls *node) {     //pass this function a pointer to the ghoul node to delete

            if(node->prev == NULL){
                if (node->next == NULL){
                    ghoulHead = NULL;
                } 

                else {
                    node->next->prev = NULL;
                    ghoulHead = node->next;
                }
            } 

            else {
                if (node->next == NULL){
                    node->prev->next = NULL;
                } 

                else {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
            }
			
            delete node;
            node = NULL;

        }

        Block *createBlock() {
            
            Block *bh = new Block;
            bh->next = blockHead;
            if (blockHead != NULL)
                blockHead->prev = bh;
            blockHead = bh;
            nBlocks++;
            return bh;
        }
        Items *createItem() {
            
            Items *it = new Items;
            it->next = itemHead;
            if (itemHead != NULL)
                itemHead->prev = it;
            itemHead = it;
            nItems++;
            return it;
        }
        Items *createHeart() {
            
            Items *it = new Heart;
            it->next = itemHead;
            if (itemHead != NULL)
                itemHead->prev = it;
            itemHead = it;
            nItems++;
            return it;
        }
        Items *createDiamond() {
            
            Items *it = new Diamond;
            it->next = itemHead;
            if (itemHead != NULL)
                itemHead->prev = it;
            itemHead = it;
            nItems++;
            return it;
        }
        Items *createDagger() {
            
            Items *it = new Dagger;
            it->next = itemHead;
            if (itemHead != NULL)
                itemHead->prev = it;
            itemHead = it;
            nItems++;
            return it;
        }
        Items *createAxe() {
            
            Items *it = new Axe;
            it->next = itemHead;
            if (itemHead != NULL)
                itemHead->prev = it;
			itemHead = it;
            nItems++;
            return it;
        }
        void deleteDagger(P_Dagger *node) {
			if(node->prev == NULL){
                if (node->next == NULL){
                    dagHead = NULL;
                } 
                else {
                    node->next->prev = NULL;
                    dagHead = node->next;
                }
            } 
            else {
                if (node->next == NULL){
                    node->prev->next = NULL;
                } 
                else {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
            }
            //delete node;
			nDags--;
            node = NULL;
        }
        void deleteAxe(P_Axe *node) {
			if(node->prev == NULL){
                if (node->next == NULL){
                    axeHead = NULL;
                } 
                else {
                    node->next->prev = NULL;
                    axeHead = node->next;
                }
            } 
            else {
                if (node->next == NULL){
                    node->prev->next = NULL;
                } 
                else {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
            }
            //delete node;
			nAxes--;
            node = NULL;
        }
        void deleteItem(Items *node) {
			if(node->prev == NULL){
                if (node->next == NULL){
                    itemHead = NULL;
                } 
                else {
                    node->next->prev = NULL;
                    itemHead = node->next;
                }
            } 
            else {
                if (node->next == NULL){
                    node->prev->next = NULL;
                } 
                else {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
            }
            //delete node;
			nItems--;
            node = NULL;
        }




        char **getLevel(char *lvl, int rows, int cols) {

            char **array;
            array = new char*[rows];
            ifstream inFile;
            inFile.open(lvl);
            for (int r = 0; r < rows; r++) {

                array[r] = new char[cols];
                int c = 0;
                char ch = inFile.get();
                while (ch != '\n') {
                    array[r][c] = ch;
                    c++;
                    ch = inFile.get();
                }
            }

            return array;    
        }
};

#endif //_GAME_H_
