#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>
#include <random>
// Game specific includes
#include "coursework.h"


using namespace std;

class cGame
{
public:
	cGame();

	void Initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void Run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, bool loop);
	void CleanUp(SDL_Window* theSDLWND);
	void Render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, bool loop);
	void Render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void Update();
	void Update(double deltaTime, double spawnTime, bool theLoop);
	bool GetInput(bool theLoop);
	double GetElapsedSeconds();
	bool loop;

	static cGame* GetInstance();

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	duration< double > spawningTime;

	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cPlayer thePlayer;
	cEnemy theEnemy;
	cBullet theBullet;
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cEnemy*> theEnemies;
	vector<cBullet*> theBullets;
	vector<cSprite*> theExplosions;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Game objects
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	int theScore;
	string strScore;
	cTexture* tempTextTexture;
	
	//variable for gamestates
	gameState theGameState;

	//variables for buttons
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	SDL_Rect pos;
	FPoint scale;
	SDL_Point theAreaClicked;
	
	//variables for file handling
	string hScore;
	int highscore;
	bool changeHighScore = false;
};

#endif
