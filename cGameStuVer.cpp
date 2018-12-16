/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include <fstream>
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::GetInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::Initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	//Reset score value
	theScore = 0;

	// Store the textures
	textureName = { "Enemy", "bullet","player","theBackground", "explosion", "gameOverScreen", "title"};
	texturesToUse = { "Images\\Sprites\\Enemy2.png", "Images\\Sprites\\Bullet.png", "Images\\Sprites\\Player.png", "Images\\Bkg\\Background.png", "Images\\Sprites\\explosion.png", "Images\\Bkg\\Background2.png", "Images\\Bkg\\Title.png" };
	for (int tCount = 0; tCount < (int)textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	//Store the button textures
	btnNameList = { "exit_btn", "instructions_btn", "play_btn", "menu_btn",};
	btnTexturesToUse = { "Images/Buttons/ExitButton.png", "Images/Buttons/HelpButton.png", "Images/Buttons/PlayButton.png", "Images/Buttons/MenuButton.png"};
	btnPos = { { WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT / 3 * 2 }, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2 }, { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3 * 2  }, { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3 * 2 }};
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}

	//Set the gamestate to the menu for the start of the game
	theGameState = gameState::menu;

	//Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge", "pacifico" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/Pacifico.ttf" };
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 75);
	}

	//Create the text used in the game
	gameTextList = { "Space Invaders", "Score : ", "Game Over", "Spacebar to Shoot", "Arrow keys to Move", "Escape to end game early", "New Highscore!!", "Current HighScore :" };
	strScore = gameTextList[1];
	strScore += to_string(theScore).c_str();

	hScore = gameTextList[7];

	//File handling to get the current highscore
	ifstream InputFile("scores.txt");
	if (InputFile.is_open())
	{
		InputFile >> highscore;
		hScore += to_string(highscore).c_str();
		InputFile.close();
	}
	
	//Adding each text texture
	theTextureMgr->addTexture("Title", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, gameTextList[0], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("gameOver", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, gameTextList[2], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("shoot", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, gameTextList[3], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("move", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, gameTextList[4], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("end", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, gameTextList[5], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("newHs", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, gameTextList[6], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("oldHs", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, hScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme", "shot", "explosion", "button" };
	soundTypes = { soundType::music, soundType::sfx, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/Coursework music.wav", "Audio/Coursework shot.wav", "Audio/explosion2.wav", "Audio/Coursework button.wav" };
	for (int sounds = 0; sounds < (int)soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	//Plays the background music
	theSoundMgr->getSnd("theme")->play(-1);

	//Creates the player
	thePlayer.setSpritePos({(WINDOW_WIDTH / 2) , 656});
	thePlayer.setTexture(theTextureMgr->getTexture("player"));
	thePlayer.setSpriteDimensions(theTextureMgr->getTexture("player")->getTWidth(), theTextureMgr->getTexture("player")->getTHeight());
	thePlayer.setPlayerVelocity(300);
	thePlayer.setSpriteTranslation({ 50,50 });
}

void cGame::Run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, bool loop)
{
	//Run the game
	loop = true;

	//Repeats whilst the game is running
	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->GetElapsedSeconds();

		loop = this->GetInput(loop);
		double spawnsTime = 0;
		spawnsTime += elapsedTime;
		this->Update(elapsedTime, spawnsTime, loop);
		this->Render(theSDLWND, theRenderer, loop);
	}
}

void cGame::Render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, bool loop)
{
	SDL_RenderClear(theRenderer);

	//Allow the change of gamestates
	switch (theGameState)
	{
	case gameState::menu:
	{
		//Sets the background for the game
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("title"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("title")->getTWidth(), theTextureMgr->getTexture("title")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Displays the title of the game
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 300, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h};
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theScore = -100;

		// Render Buttons
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 400, 300 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("instructions_btn")->setSpritePos({ 400, 400 });
		theButtonMgr->getBtn("instructions_btn")->render(theRenderer, &theButtonMgr->getBtn("instructions_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("instructions_btn")->getSpritePos(), theButtonMgr->getBtn("instructions_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 500 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	
		//If button is clicked
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		theGameState = theButtonMgr->getBtn("instructions_btn")->update(theGameState, gameState::instructions, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);
	}
	break;
	case gameState::instructions:
	{
		//Sets the background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("title"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("title")->getTWidth(), theTextureMgr->getTexture("title")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Sets the instructions text on the screen
		cTexture* tempTextTexture = theTextureMgr->getTexture("shoot");
		SDL_Rect pos = { 100, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		cTexture* tempTextTexture2 = theTextureMgr->getTexture("move");
		SDL_Rect pos2 = { 100, 200, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale);

		cTexture* tempTextTexture3 = theTextureMgr->getTexture("end");
		SDL_Rect pos3 = { 100, 300, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos3, scale);
	
		//Render Button
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 700, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());

		//If the button is pressed
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
		
	}
	break;
	case gameState::playing:
	{
		//Sets the background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render each enemy in the vector array
		for (int draw = 0; draw < (int)theEnemies.size(); draw++)
		{
			theEnemies[draw]->render(theRenderer, &theEnemies[draw]->getSpriteDimensions(), &theEnemies[draw]->getSpritePos(), theEnemies[draw]->getSpriteRotAngle(), &theEnemies[draw]->getSpriteCentre(), theEnemies[draw]->getSpriteScale());
		}
		// Render each bullet in the vector array
		for (int draw = 0; draw < (int)theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}
		// Render each explosion in the vector array
		for (int draw = 0; draw < (int)theExplosions.size(); draw++)
		{
			theExplosions[draw]->render(theRenderer, &theExplosions[draw]->getSourceRect(), &theExplosions[draw]->getSpritePos(), theExplosions[draw]->getSpriteScale());
		}

		//Checks to see if the previous highscore has been beaten
		if (changeHighScore)
		{
			hScore = gameTextList[7];
			hScore += to_string(highscore).c_str();
			theTextureMgr->deleteTexture("oldHs");
			theTextureMgr->addTexture("oldHs", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, hScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
			changeHighScore = false;
		}

		//Displays the current highscore
		cTexture* tempTextTexture = theTextureMgr->getTexture("oldHs");
		SDL_Rect pos = { 10 , 10, tempTextTexture->getTextureRect().w / 2, tempTextTexture->getTextureRect().h / 2 };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Renders the player's score
		cTexture* tempScoreTexture = theTextureMgr->getTexture("theScore");
		SDL_Rect pos2 = { 800, 10, tempScoreTexture->getTextureRect().w / 2, tempScoreTexture->getTextureRect().h / 2};
		FPoint scale2 = { 1, 1 };
		tempScoreTexture->renderTexture(theRenderer, tempScoreTexture->getTexture(), &tempScoreTexture->getTextureRect(), &pos2, scale2);

		// render the player
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());
		

		/* Let the computer pick a random number */
		random_device rd;    // non-deterministic engine 
		mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
		uniform_int_distribution<> EnemyDis{ 4, 10 };

		//Checks to see if there are any enemies left and will trigger if there are none
		if (theEnemies.size() == 0)
		{
			//Renders a random number of enemies taken from the random number generator
			for (int enemynum = 0; enemynum < EnemyDis(gen); enemynum++)
			{
				theEnemies.push_back(new cEnemy);
				theEnemies[enemynum]->setSpritePos({ (100 * enemynum), 100 });
				theEnemies[enemynum]->setSpriteTranslation({ 100, -50 });
				theEnemies[enemynum]->setTexture(theTextureMgr->getTexture("Enemy"));
				theEnemies[enemynum]->setSpriteDimensions(theTextureMgr->getTexture("Enemy")->getTWidth(), theTextureMgr->getTexture("Enemy")->getTHeight());
				theEnemies[enemynum]->setActive(true);
			}

			//Increases the score due to a wave being completed
			theScore += 100;
			strScore = gameTextList[1];
			strScore += to_string(theScore).c_str();
			theTextureMgr->deleteTexture("theScore");
			theTextureMgr->addTexture("theScore", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
		}

		//Checks to see of the player has beaten the target score
		if (theScore >= 3000)
		{
			for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
			{
				if ((*enemyIterator)->isActive())
				{
					(*enemyIterator)->setActive(false);
				}
			}

			theGameState = gameState::end;
		}
	}
	break;
	case gameState::end:
	{
		//Sets the background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("gameOverScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("gameOverScreen")->getTWidth(), theTextureMgr->getTexture("gameOverScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Displays the game over message
		cTexture* tempTextTexture = theTextureMgr->getTexture("gameOver");
		SDL_Rect pos = { 100, 100, tempTextTexture->getTextureRect().w / 2, tempTextTexture->getTextureRect().h / 2 };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Displays the player's score
		theTextureMgr->addTexture("theScore", theFontMgr->getFont("pacifico")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
		cTexture* tempScoreTexture = theTextureMgr->getTexture("theScore");
		SDL_Rect pos2 = { 100, 300, tempScoreTexture->getTextureRect().w / 2, tempScoreTexture->getTextureRect().h / 2 };
		FPoint scale2 = { 1, 1 };
		tempScoreTexture->renderTexture(theRenderer, tempScoreTexture->getTexture(), &tempScoreTexture->getTextureRect(), &pos2, scale2);

		//Renders the buttons
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 700, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 700, 600 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		//If the button has been pressed
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);

		//File handling to check if the highscore has been beaten
		ifstream InputFile("scores.txt");
		if (InputFile.is_open())
		{
			InputFile >> highscore;
			if (highscore > theScore)
			{
				cTexture* tempTextTexture = theTextureMgr->getTexture("oldHs");
				SDL_Rect pos = { 500, 100, tempTextTexture->getTextureRect().w / 2, tempTextTexture->getTextureRect().h / 2 };
				FPoint scale = { 1, 1 };
				tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			}
			else
			{
				highscore = theScore;
				cTexture* tempTextTexture = theTextureMgr->getTexture("newHs");
				SDL_Rect pos3 = { 500, 100, tempTextTexture->getTextureRect().w / 2, tempTextTexture->getTextureRect().h / 2 };
				FPoint scale3 = { 1, 1 };
				tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos3, scale3);
				changeHighScore = true;
				highscore = theScore;
				ofstream myfile("scores.txt");
				if (myfile.is_open())
				{
					myfile << theScore;
					myfile.close();
				}
			}
			InputFile.close();
		}
	}
	break;
	//Ends the game
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}

	SDL_RenderPresent(theRenderer);
}

void cGame::Render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{
	SDL_RenderPresent(theRenderer);
}

void cGame::Update()
{
	cout << clock();
}

void cGame::Update(double deltaTime, double spawnTime, bool loop)
{
	//Allows different gamestates
	switch (theGameState)
	{
	case gameState::playing:
	{
		// Update the visibility and position of each enemy
		vector<cEnemy*>::iterator enemyIterator = theEnemies.begin();

		while (enemyIterator != theEnemies.end())
		{
			if ((*enemyIterator)->isActive() == false)
			{
				enemyIterator = theEnemies.erase(enemyIterator);
			}
			else
			{
				(*enemyIterator)->update(deltaTime);
				//Checks to see if the enemy has got to the end of the screen and if so it will move the enemy the opposite way and move it further down the screen
				if ((*enemyIterator)->getSpritePos().x < 0)
				{
					(*enemyIterator)->setSpriteTranslation({ (*enemyIterator)->getSpriteTranslation().x * (-1),(*enemyIterator)->getSpriteTranslation().y * (-1) });
					(*enemyIterator)->setSpritePos({ 0, ((*enemyIterator)->getSpritePos().y + 100) });
				}
				else if ((*enemyIterator)->getSpritePos().x > (WINDOW_WIDTH - (*enemyIterator)->getSpritePos().w))
				{
					(*enemyIterator)->setSpriteTranslation({ (*enemyIterator)->getSpriteTranslation().x * (-1),(*enemyIterator)->getSpriteTranslation().y * (-1) });
					(*enemyIterator)->setSpritePos({ (WINDOW_WIDTH - (*enemyIterator)->getSpritePos().w), ((*enemyIterator)->getSpritePos().y + 100) });
				}

				//Checks to see if the enemy has made it so far down the screen and if so then the player loses and the game ends
				if ((*enemyIterator)->getSpritePos().y >= (WINDOW_HEIGHT - 200))
				{
					for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
					{
						if ((*enemyIterator)->isActive())
						{
							(*enemyIterator)->setActive(false);
						}
					}
					theGameState = gameState::end;
				}
				++enemyIterator;
			}
		}
		// Update the visibility and position of each bullet
		vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
		while (bulletIterartor != theBullets.end())
		{
			if ((*bulletIterartor)->isActive() == false)
			{
				bulletIterartor = theBullets.erase(bulletIterartor);
			}
			else
			{
				(*bulletIterartor)->update(deltaTime);
				++bulletIterartor;
			}
		}
		// Update the visibility and position of each explosion
		vector<cSprite*>::iterator expIterartor = theExplosions.begin();
		while (expIterartor != theExplosions.end())
		{
			if ((*expIterartor)->isActive() == false)
			{
				expIterartor = theExplosions.erase(expIterartor);
			}
			else
			{
				(*expIterartor)->animate(deltaTime);
				++expIterartor;
			}
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			//(*bulletIterartor)->update(deltaTime);
			for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
			{
				if ((*enemyIterator)->collidedWith(&(*enemyIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and enemy to false
					(*enemyIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);
					theExplosions.push_back(new cSprite);
					int index = theExplosions.size() - 1;
					theExplosions[index]->setSpriteTranslation({ 0, 0 });
					theExplosions[index]->setActive(true);
					theExplosions[index]->setNoFrames(16);
					//Displays an explosion to give the player visual recognition of destroying the enemy
					theExplosions[index]->setTexture(theTextureMgr->getTexture("explosion"));
					theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("explosion")->getTWidth() / theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("explosion")->getTHeight());
					theExplosions[index]->setSpritePos({ (*enemyIterator)->getSpritePos().x + (int)((*enemyIterator)->getSpritePos().w / 2), (*enemyIterator)->getSpritePos().y + (int)((*enemyIterator)->getSpritePos().h / 2) });

					//Plays an explosion sound to give the user audible recognition
					theSoundMgr->getSnd("explosion")->play(0);

					//Adds to the score
					theScore++;
				}
			}
		}

		// Update the Player's position
		thePlayer.update(deltaTime);

		//Creates SDL_Points to use when the player hits the end of the screen
		SDL_Point farRight = { WINDOW_WIDTH - thePlayer.getSpritePos().w, 650 };
		SDL_Point farLeft = { 0, 650 };

		//If the player would go off the screen it would teleport them to the opposite side of the screen
		if (thePlayer.getSpritePos().x < 0)
		{
			thePlayer.setSpritePos(farRight);
		}
		else if (thePlayer.getSpritePos().x > (WINDOW_WIDTH - thePlayer.getSpritePos().w))
		{
			thePlayer.setSpritePos(farLeft);
		}
	}
	break;
	//Ends the game
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
}

bool cGame::GetInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					//If the player presses the left click on the mouse then an audible sound will be played and the position of the click will be recorded
					theSoundMgr->getSnd("button")->play(0);
					theAreaClicked = { event.motion.x, event.motion.y };
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					//If the game is in play and the player presses the escape key then the gamestate will be changed to the end screen and the playable part of the game will end
					if (theGameState == gameState::playing)
					{
						//Removes any enemies that are still rendered
						for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
						{
							if ((*enemyIterator)->isActive())
							{
								(*enemyIterator)->setActive(false);
							}
						}
						theGameState = gameState::end;
					}
					break;
				case SDLK_DOWN:
				{

				}
				break;

				case SDLK_UP:
				{
				}
				break;
				case SDLK_RIGHT:
				{
					//If the game is in play and the user presses the right key then the player will move right
					if (theGameState == gameState::playing)
					{
							thePlayer.setPlayerMove(-1);
					}
				}
				break;

				case SDLK_LEFT:
				{
					//If the game is in play and the user presses the left key then the player will move left
					if (theGameState == gameState::playing)
					{
							thePlayer.setPlayerMove(1);
					}
					
				}
				break;
				case SDLK_SPACE:
				{
					//If the game is in play and the user presses the spacebar then the player will shoot
					if (theGameState == gameState::playing)
					{
							theBullets.push_back(new cBullet);
							int numBullets = theBullets.size() - 1;

							theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + thePlayer.getSpriteCentre().x - 16, thePlayer.getBoundingRect().y + thePlayer.getSpriteCentre().y });
							theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
							theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
							theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
							theBullets[numBullets]->setBulletVelocity(50);
							theBullets[numBullets]->setSpriteRotAngle(thePlayer.getSpriteRotAngle());
							theBullets[numBullets]->setActive(true);
					}
				}
				theSoundMgr->getSnd("shot")->play(0);
				break;
				default:
					break;
				}

			default:
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_RIGHT:
				{
					//When the user lets go of the right key then the player will stop moving
					thePlayer.setPlayerMove(0);
				}
				break;

				case SDLK_LEFT:
				{
					//When the user lets go of the left key then the player will stop moving
					thePlayer.setPlayerMove(0);
				}
				case SDLK_SPACE:
				{
				}
				}
		}

		//Checks to see if the game state is in the quit section and if so then the game ends
		if (theGameState == gameState::quit)
		{
			theLoop = false;
		}

	}
	return theLoop;
}

double cGame::GetElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::CleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	//SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}


