#pragma once
#include <cmath>
#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <stack>
#include <fstream>

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

class Player;
class Enemy;
class Game;

/// <summary>
/// This struct fors the back bone of the maze be forming the basis for its components
/// </summary>
static struct Cell
{
	bool visited;
	bool topWall;
	bool bottomWall;
	bool leftWall;
	bool rightWall;
	bool isPath;
	bool m_playerPath;
	bool m_enemyPath;

	bool breakable;

	int m_playerTracks = 0;
};

class MazeGenerator
{
public:
	const static int MAZESIZE = 51;

	MazeGenerator(Game & t_game, Player& t_player, Enemy& t_enemy);
	~MazeGenerator();
	bool getWall();
	void drawMaze(sf::RenderWindow & t_window);
	void Initialize();
	/// <summary>
	/// This carves a maze out of the cells through the use of stacks, while loops and a enum clas
	/// Additionally it adds on the entrance for the player
	/// </summary>
	void GenerateMaze();
	void setPath(sf::Texture &t_texture);

	void generateWalls(sf::Texture &t_northTexture, sf::Texture &t_southTexture, sf::Texture &t_eastTexture, sf::Texture &t_westTexture, sf::Texture &t_northBTexture, sf::Texture &t_southBTexture, sf::Texture &t_eastBTexture, sf::Texture &t_westBTexture, sf::Texture &t_northCTexture, sf::Texture &t_southCTexture);

	//This function is used to set up the enemy within the maze
	void setEnemy();

	//This is used to check for collisions between the player and the maze and to find the tiles that below to the player and enemy
	void update();
	void calculateIntersectPoints(int t_x, int t_y);

	//This breaks a few walls before hand to create "islands" with in the maze to create more of a challenge
	void createIslands();

	//This function is used to render a set number of walls breakable
	void createBreakableWalls();

	//This breaks the walls that are breakable under the right circmstances
	void breakWall();

	//This clears out unnecessary sprites by turning off their associated bools
	void cleanMaze();

	//The below 2 functions are used to find the tile the player and enemy are on
	void findPlayerTile();
	//This is also used to help the enemy navigate
	void findEnemyTile();

	sf::Vector2f m_startPosition;
	sf::Sprite m_path[MAZESIZE][MAZESIZE];

	sf::Sprite m_northWallSprites[MAZESIZE][MAZESIZE];
	sf::Sprite m_southWallSprites[MAZESIZE][MAZESIZE];
	sf::Sprite m_eastWallSprites[MAZESIZE][MAZESIZE];
	sf::Sprite m_westWallSprites[MAZESIZE][MAZESIZE];

	sf::Sprite m_brokenNorthWallSprites[MAZESIZE][MAZESIZE];
	sf::Sprite m_brokenSouthWallSprites[MAZESIZE][MAZESIZE];
	sf::Sprite m_brokenEastWallSprites[MAZESIZE][MAZESIZE];
	sf::Sprite m_brokenWestWallSprites[MAZESIZE][MAZESIZE];

	sf::Sprite m_northCornerSprite[MAZESIZE][MAZESIZE];
	sf::Sprite m_southCornerSprite[MAZESIZE][MAZESIZE];

	//The base components of the maze
	Cell m_map[MAZESIZE][MAZESIZE];
	sf::Vector2f getStartPosition();

	sf::Vector2i getPlayerTile();
	sf::Vector2i getEnemyTile();

	//Used to determine when the enemy starts running 
	int m_highestPTrack;

  private:

	int m_startRow;
	int m_startColumn;
	int m_nextRow;
	int m_nextCol;

	//This ints are used to help render the player and enemy(also to help the enemy move)
	int m_pX, m_pY;
	int m_eX, m_eY;
	int m_eRX, m_eRY;

	//These ints are used create islands and breakable walls
	int m_isleCreate = 10;
	int m_breakableWalls = 10;

	Player& m_player;
	Enemy& m_enemy;

	bool m_maze[MAZESIZE][MAZESIZE];
	sf::Vector2f m_groundPosition[MAZESIZE][MAZESIZE];

	sf::CircleShape m_wallCircles[MAZESIZE][MAZESIZE];
	sf::Vector2f m_wCirclesCenter[MAZESIZE][MAZESIZE];

	sf::Vector2f m_circleIntersectPoint[2][4];

	float m_wCircleRadius = 75;

	Game& m_game;

	sf::Vector2f m_currentPosition;
	//These stacks are used to help create the maze in order to prevent dead ends from stalling the process
	std::stack<int> m_backTrackX, m_backTrackY;
	void drawWalls(sf::RenderWindow & t_window, int row, int col);
};

