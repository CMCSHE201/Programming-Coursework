#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

// SDL2 Headers
#include <SDL.h>

#include "cSDL2WNDManager.h"
#include "cGame.h"

using namespace std;

//This is the SDL window
static cSDL2WNDManager* pgmWNDMgr = cSDL2WNDManager::getInstance();

//This is the our Game
static cGame* theGame = cGame::GetInstance();

int main(int argc, char *argv[])
{
	if (!pgmWNDMgr->initWND(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT))
		return -1;

	theGame->Initialise(pgmWNDMgr->getSDLWindow(), pgmWNDMgr->getSDLRenderer());

	theGame->Run(pgmWNDMgr->getSDLWindow(), pgmWNDMgr->getSDLRenderer(), true);

	theGame->CleanUp(pgmWNDMgr->getSDLWindow());

	return 0;
}