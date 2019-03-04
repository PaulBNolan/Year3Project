#pragma once
#include <SFML/Graphics.hpp>
#include "KeyHandler.h"
#include "Game.h"
#include "Pause.h"
#include "MazeGenerator.h"
#include "Enums.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

class Game;
class XboxController;
class Pause;
class MazeGenerator;
class Enemy
{
public:
	Enemy(Game& t_game, sf::Texture const & t_texture, sf::Texture const & t_SEtexture, sf::Texture const & t_NWtexture, sf::Texture const & t_SERtexture, sf::Texture const & t_NWRtexture);
	~Enemy();
	void update(double t_time,MazeGenerator & t_maze);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	//Collision detection between player and enemy to end game
	bool collisionCheck(sf::CircleShape t_pCircle, sf::Vector2f t_pCircleCenter);
	//Enum for enemy movement
	EnemyDirection m_eDirection;
	void changeDirection(EnemyDirection t_direction);

	float m_eCircleRadius = 50;
private:
	void calculateVelocity();
	sf::Vector2f m_velocity;

	void initSprites();
	//This function is used to move the enemy and leads into the below 2 functions
	void movement(MazeGenerator & t_maze);
	/// <summary>
	/// This function is used to look for traces of the players presence when the enemy visits junctions
	/// The enemy will go the path with higher player_tracks value
	/// </summary>
	void checkForPlayerTracks(MazeGenerator & t_maze);
	/// <summary>
	/// This function is used to turn the enemy when they head into a wall and will turn them based on the surrounding walls
	/// </summary>
	/// <param name="t_maze"></param>
	void enemyTurn(MazeGenerator & t_maze);
	/// <summary>
	/// This finction is used to break walls when the player is on the opposite side while the enemy
	/// is on the other
	/// </summary>
	/// <param name="t_maze"></param>
	void breakWalls(MazeGenerator & t_maze);
	//This function is used to animate the anime every 6th of a second
	void animate();
	//This function is used to cause the enemy to run based on how close they are to the plaer
	void startRunning(MazeGenerator & t_maze);

	sf::Vector2f m_enemyPosition;
	sf::Vector2f m_enemyTexturePosition;

	double m_animationTime = 0;
	sf::Vector2f m_spriteSheetPositions[6][5];
	int m_movementType = 0; //This int is used to control which animation is used
	int m_animationStage = 0;

	sf::Sprite m_enemy;
	int m_spriteWidth[5], m_spriteHeight[5];
	bool m_collided;
	bool m_running = false;

	sf::CircleShape m_enemyCircle;
	sf::Vector2f m_enemyCenter;

	sf::Texture const & m_texture;
	sf::Texture const & m_seTexture;
	sf::Texture const & m_nwTexture;

	sf::Texture const & m_seRunningTexture;
	sf::Texture const & m_nwRunningTexture;

	Game& m_game;
};

