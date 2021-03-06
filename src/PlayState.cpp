/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include <cmath>
#include "CGame.h"
#include "PlayState.h"

PlayState PlayState::m_PlayState;

using namespace std;

#define SPEED 150

void PlayState::init()
{
	vertical = 1;
	count = 0;
	background = new CImage();
    background->loadImage("data/maps/black.png");
	while(count != 7){
		CSprite* enemy = new CSprite();
		enemy->loadSpriteSparrowXML("data/img/bug.xml");
		if(count > 2) vertical = 3;
		enemy->setPosition((count%3)*125,50*vertical);
		enemy->setAnimRate(0);
		enemies[count] = enemy;
		count++;
	}
    player = new CSprite();
    player->loadSpriteSparrowXML("data/img/spaceship.xml");
    player->setPosition(172,550);
    player->setAnimRate(0);
    dirx = 0; // direção do personagem: para a direita (5), esquerda (-5)
    diry = 0; // direção do personagem: para cima (5), baixo (-5)

    keyState = SDL_GetKeyState(0); // get key state array
    cout << "PlayState Init Successful" << endl;
}

void PlayState::cleanup()
{
    delete player;
	cout << "PlayState Clean Successful" << endl;
}

void PlayState::pause()
{
	cout << "PlayState Paused" << endl;
}

void PlayState::resume()
{
	cout << "PlayState Resumed" << endl;
}

void PlayState::handleEvents(CGame* game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->quit();
				break;

            case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        game->quit();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_VIDEORESIZE:
                game->resize(event.resize.w, event.resize.h);
        }
        dirx = keyState[SDLK_RIGHT]*SPEED + keyState[SDLK_LEFT]*-SPEED;
        diry = keyState[SDLK_DOWN] *SPEED + keyState[SDLK_UP]  *-SPEED;
    }

    player->setXspeed(dirx);
    //player->setYspeed(diry);

    if(dirx || diry)
        player->setAnimRate(30);
    else {
        player->setAnimRate(0);
        player->setCurrentFrame(3);
    }

    if(dirx > 0)
        player->setMirror(false);
    else if(dirx < 0)
        player->setMirror(true);
}

void PlayState::update(CGame* game)
{
    int x = (int) enemies[0]->getX();
	count = 0;
	while(count != 7){
		if(x == 0) enemies[count]->setXspeed(100);
		else if(x == 100) enemies[count]->setXspeed(-100);
		enemies[count]->setAnimRate(30);
		count++;
	}
    count = 0;
    while(count != 7){
    	enemies[count]->update(game->getUpdateInterval());
    	count++;
    }
    player->update(game->getUpdateInterval());
}

void PlayState::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //player->setRotation(0);
    //player->setScale(1);
    //player->draw();

    // Agora o mapa e' responsavel por desenhar o jogador, imediatamente depois
    // da camada 0 (primeira camada do mapa)
    count = 0;
    background->draw();
    player->draw();
    while(count != 7){
    	enemies[count]->draw();
    	count++;
    }

    SDL_GL_SwapBuffers();
}
