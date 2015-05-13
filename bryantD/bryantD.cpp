/*I also worked on Game.h
 *Even though I mainly worked on this 
 *
 *Might make this into a templated class later on
 * */

#include "Game.h"
#include <X11/keysym.h>
#include <cmath>

int x=1250, y=900;
extern int dash = 0;
const float gravity = 0.5f;

void playerFwd(Game *g) 
{	
    //player determines if player is moving forward
    if(g->player.getVelX() > 0) {
        g->player.setFwd(true);
    } else if(g->player.getVelX() < 0) {
        g->player.setFwd(false);
    }

    if(g->getRun()) {
        g->player.setFwd(true);
    }
}

void playerJump(Game *g) 
{
    //check if plalyer is jumping
    if (g->player.getVelY() == 0 && g->getCol()) {
        g->player.setJump(false);
        g->setCol(false);
    } else {
        g->player.setJump(true); 
    }
}

void playerForces(Game *g) 
{
    //apply gravity
    g->player.applyGrav(gravity);
    //apply velocity in both directions
    g->player.setPosX(g->player.getPosX() + g->player.getVelX());	
    g->player.setPosY(g->player.getPosY() + g->player.getVelY());
    //reset x velocity 
    g->player.setVelX(0);
}

void ghoulForces(Ghouls *gh) 
{
    //apply gravity
    gh->applyGrav(gravity);
    //apply velocity in both directions
    gh->setPosX(gh->getPosX() + gh->getVelX());	
    gh->setPosY(gh->getPosY() + gh->getVelY());
    //reset x velocity 
    gh->setVelX(0);
}

void characterCollision(Game *g) 
{
    //Check for collision with obstacles
    float playerW = g->player.getWidth();
    float playerH = g->player.getHeight();
    float playerY = g->player.getPosY();
    float playerX = g->player.getPosX();

    float blockY;
    float blockX;
    float blockH;
    float blockW;

    Block *block = g->blockHead;

    while(block) {
        blockY = block->getCenterY();
        blockX = block->getCenterX();
        blockH = block->getHeight();
        blockW = block->getWidth();

        //top
        if ( (playerX > blockX - blockW - playerW 
                    && playerX < blockX + blockW + playerW) 
                && (playerY <= blockY + blockH + playerH) 
                && (playerY >= (blockY + blockH)) 
                && !(block->getWall()) ) {
            g->player.setVelY(0);
            g->player.setPosY(blockY + blockH + playerH);
            g->setCol(true);
        }

        //bottom
        if ( (playerX > blockX - blockW - playerW 
                    && playerX < blockX + blockW + playerW) 
                && (playerY >= blockY - blockH - playerH) 
                && (playerY <= (blockY - blockH)) 
                && !(block->getWall()) ) {
            g->player.setVelY(0);
            g->player.setPosY(blockY - blockH - playerH);

            if (block->getClimb()) {
                g->setCol(true);
            }
        }

        //left side
        if ( (playerX > (blockX - blockW - playerW) 
                    && (playerX < blockX)) 
                && (playerY < (blockY + blockH))
                && (playerY > (blockY - blockH))) {
            g->player.setPosX(blockX - blockW - playerW);
        }

        //right side
        if ( playerX < (blockX + blockW + playerW) 
                && (playerX > blockX) 
                && (playerY < (blockY + blockH ))
                && (playerY > (blockY - blockH ))) {
            g->player.setPosX(blockX + blockW + playerW);
        }
        block = block->next;
    }
}

void ghoulCollision(Game *g, Ghouls *gh) 
{
    //Check for collision with obstacles
    float ghoulW = gh->getWidth();
    float ghoulH = gh->getHeight();
    float ghoulY = gh->getPosY();
    float ghoulX = gh->getPosX();

    float blockY;
    float blockX;
    float blockH;
    float blockW;

    float dist;

    Block *block = g->blockHead;

    ghoulY = gh->getPosY();
    ghoulX = gh->getPosX();

    while(block) {

        blockY = block->getCenterY();
        blockX = block->getCenterX();
        blockH = block->getHeight();
        blockW = block->getWidth();

        dist = abs(blockX) - abs(ghoulX);

        if (block->getEdge() && dist == 0) { 
            if (gh->getCol() == 0) {
                gh->setCol(1);
            } else {
                gh->setCol(0);
            }
        }

        //top
        if ( (ghoulX > blockX - blockW - ghoulW 
                    && ghoulX < blockX + blockW + ghoulW) 
                && (ghoulY <= blockY + blockH + ghoulH) 
                && (ghoulY >= (blockY + blockH)) 
                && !(block->getWall())) {
            gh->setVelY(0);
            gh->setPosY(blockY + blockH + ghoulH);
        }

        //bottom
        if ( (ghoulX > blockX - blockW - ghoulW 
                    && ghoulX < blockX + blockW + ghoulW) 
                && (ghoulY >= blockY - blockH - ghoulH) 
                && (ghoulY <= (blockY - blockH)) 
                && !(block->getWall())) {
            gh->setVelY(0);
            gh->setPosY(blockY - blockH - ghoulH);
        }

        //left side
        if ( (ghoulX > (blockX - blockW - ghoulW) 
                    && (ghoulX < blockX)) 
                && (ghoulY < (blockY + blockH))
                && (ghoulY > (blockY - blockH))) {
            gh->setPosX(blockX - blockW - ghoulW);

            if (gh->getCol() == 0) {
                gh->setCol(1);
            } else {
                gh->setCol(0);
            }
        }

        //right side
        if ( ghoulX < (blockX + blockW + ghoulW) 
                && (ghoulX > blockX) 
                && (ghoulY < (blockY + blockH ))
                && (ghoulY > (blockY - blockH ))) {
            gh->setPosX(blockX + blockW + ghoulW);

            if (gh->getCol() == 0) {
                gh->setCol(1);
            } else { 
                gh->setCol(0);
            }
        }
        block = block->next;
    }
}

void hitDetection(Game *g, Ghouls *gh) 
{
    //Check for collision with obstacles
    float ghoulW = gh->getWidth();
    float ghoulH = gh->getHeight();
    float ghoulY = gh->getPosY();
    float ghoulX = gh->getPosX();

    float playerW = g->player.getWidth();
    float playerH = g->player.getHeight();
    float playerY = g->player.getPosY();
    float playerX = g->player.getPosX();

    //top
    if ( (ghoulX > playerX - playerW - ghoulW 
                && ghoulX < playerX + playerW + ghoulW) 
            && (ghoulY <= playerY + playerH + ghoulH) 
            && (ghoulY >= (playerY + playerH)) ) {
        g->player.setVelY(-10);
        g->player.decHealth();
    }

    //bottom
    if ( (ghoulX > playerX - playerW - ghoulW 
                && ghoulX < playerX + playerW + ghoulW) 
            && (ghoulY >= playerY - playerH - ghoulH) 
            && (ghoulY <= (playerY - playerH)) 
            /*&& !(block->getAdjDown())*/) {
        g->player.setVelY(10);
        g->player.decHealth();
    }

    //left side
    if ( (ghoulX > (playerX - playerW - ghoulW) 
                && (ghoulX < playerX)) 
            && (ghoulY < (playerY + playerH))
            && (ghoulY > (playerY - playerH))) {
        g->player.setVelX(100);
        g->player.setVelY(5);
        g->player.decHealth();
    }

    //right side
    if ( ghoulX < (playerX + playerW + ghoulW) 
            && (ghoulX > playerX) 
            && (ghoulY < (playerY + playerH ))
            && (ghoulY > (playerY - playerH ))) {
        g->player.setVelX(-100);
        g->player.setVelY(5);
        g->player.decHealth();
    }
}

void ghoulMovement(Ghouls *gh) 
{
    //check keys pressed now`
    if (gh->getCol() == 0) {
        gh->setVelX(4);
    } else {
        gh->setVelX(-4);
    }
}

void applyKey(Game *g, int *keys) {

    //check keys pressed now`
    if (keys[XK_Left] || keys['a']) {
        if(dash) {
            g->player.setVelX(g->player.getVelX() - 12);
        } else {
            g->player.setVelX(g->player.getVelX() - 4);
        }
    }

    if (keys[XK_Right] || keys['d']) {
        if(dash) {
            g->player.setVelX(g->player.getVelX() + 12);
        } else {
            g->player.setVelX(g->player.getVelX() + 4);
        }
    }

    if (keys[XK_space] || keys['w'] || keys[XK_Up]) {
        if (!g->player.getJump()) {
            if(dash) {
                g->player.setVelY(g->player.getVelY() + 18);
            }else {
                g->player.setVelY(g->player.getVelY() + 10);
            }
            g->player.setJump(true);
            g->setCol(false);
        }
    }
}


