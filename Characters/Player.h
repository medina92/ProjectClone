//#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "Character.h"
//Here is where we will add more features to the player. Work on moving whip here and other weapons here.
//Rpg elements such as a level and stats will be worked on if time is allowed.    

class Players : public Character {

        public:

                Vec dir;        //direction of player character on key press. 
                bool jump;
				int daggers;
				int axes;
				bool hasDaggers;
				bool hasAxes;
				bool dagActive;                
				bool axeActive;                
                Players() {
                        width = 30.0f;
                        height = 50.0f;
                        forward=true;
                        setHealth(15);
						setScore(0);
						vel[0] = 0;
                        vel[1] = 0;
						hasDaggers = false;
						hasAxes = false;
						dagActive = false;
						axeActive = false;
						daggers = 0;
						axes = 0;
                }
				void switchActive(){
					dagActive = !dagActive;
					axeActive = !axeActive;
					if (hasDaggers == true && hasAxes == false) {
						dagActive = true;
						axeActive = false;
					}
					if (hasDaggers == false && hasAxes == true) {
						axeActive = true;
						dagActive = false;
					}
				}
				void setDagActive(){
					dagActive = true;
					axeActive = false;
				}
				void setAxeActive(){
					axeActive = true;
					dagActive = false;
				}

                void setJump(bool iJump) { jump = iJump; }
                
                bool getJump() { return jump; }
                

};

#endif //_PLAYER_H_
