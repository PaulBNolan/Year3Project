#pragma once
#include "MazeGenerator.h"
#include "XBOXCONTROLLER.h"
#include "Enums.h"
#include "Licence.h"
#include "Menu.h"
#include "Option.h"
#include "Tutorial.h"
#include "Player.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

class Licence;
class Menu;
class Player;
class Enemy;
class Option;
class MazeGenerator;
class Tutorial;
class XboxController;
class Game
{
public:
	Game();
	~Game();
	void run();
	GameStates getGameStates() { return m_gameStates; }
	void setGameStates(GameStates t_newState) { m_gameStates = t_newState; }

	WallDirection getDirection() { return m_direction; }
	void setDirection(WallDirection t_newDirection) { m_direction = t_newDirection; }
protected:
	sf::Sprite m_backgroundSprite;
	sf::Texture m_bgTexture;

	sf::Texture m_playerIdleTexture;
	sf::Texture m_playerSouthTexture;
	sf::Texture m_playerNorthTexture;
	sf::Texture m_playerNeNwTexture;
	sf::Texture m_playerSeSwTexture;
	sf::Texture m_playerEWTexture;

	sf::Texture m_pathTexture;
	sf::Texture m_northWallTexture;
	sf::Texture m_southWallTexture;
	sf::Texture m_eastWallTexture;
	sf::Texture m_westWallTexture;
	sf::Texture m_brokenNorthWallTexture;
	sf::Texture m_brokenSouthWallTexture;
	sf::Texture m_brokenEastWallTexture;
	sf::Texture m_brokenWestWallTexture;
	sf::Texture m_northCornerTexture;
	sf::Texture m_southCornerTexture;

	sf::Texture m_enemyTexture;
	sf::Texture m_enemyTextureSouthEast;
	sf::Texture m_enemyTextureNorthWest;
	sf::Texture m_enemyRTextureSouthEast;
	sf::Texture m_enemyRTextureNorthWest;

	sf::Font m_ArialBlackfont;

	GameStates m_gameStates;
	WallDirection m_direction;

	sf::RenderWindow m_window; // main SFML window

	Licence *m_licence;
	Menu *m_menu;
	Player *m_player;
	Enemy *m_enemy;
	KeyHandler m_keyhandler;
	MazeGenerator *m_testMaze;
	Tutorial *m_tutorial;
	Option *m_options;
	XboxController m_controller;

	void processEvents();
	void processGameEvents(sf::Event& event);
	void update(sf::Time t_deltaTime);
	void render();
};

