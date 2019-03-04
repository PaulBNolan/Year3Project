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
class Player
{
public:
	Player(Game& t_game, sf::Texture const & t_iTexture, sf::Texture const & t_sTexture, sf::Texture const & t_nTexture, sf::Texture const & t_neNwTexture, sf::Texture const & t_seSwTexture, sf::Texture const & t_ewTexture,sf::RenderWindow& t_window, sf::Vector2f t_pos, KeyHandler const & t_keyHandler, XboxController& t_controller, sf::Font t_font);
	~Player(); 
	void update(double t,MazeGenerator & t_maze);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	//This function is used to handle key events such as movement and breaking walls
	void handleKeyInput(MazeGenerator & t_maze);
	sf::Sprite getPlayer() const;
	bool getPaused();
	void renderPauseScreen(sf::RenderWindow & t_window);
	//Function for circle collions
	void collisionCheck(sf::CircleShape t_wCircle, sf::Vector2f t_wCircleCenter);
	//Function to create the players velocity
	void calculateVelocity();
	//This function is used to animate the player every 8th of a second
	void animate();

	PlayerDirection m_playerDirection;
	PlayerDirection getPDirection() { return m_playerDirection; }
	void setPDirection(PlayerDirection t_newDirection);

	sf::Vector2f m_playerPosition;
	sf::Vector2f m_playerCenter;
	sf::CircleShape m_playerCircle;
private:
	void initSprites(sf::Vector2f const & pos);
	sf::Vector2f m_velocity;
	sf::Sprite m_player;
	bool m_collided;

	float m_pCircleRadius = 50;

	//The below values and textures are used to control the player animations
	double m_animationTime = 0;
	sf::Vector2f m_spriteSheetPositions[8][6];
	int m_movementType = 0;
	int m_animationStage = 0;

	//These are used for the sprite and widths of the sprite under the animations
	int m_spriteWidth[6], m_spriteHeight[6];

	sf::Texture const & m_idleTexture;

	sf::Texture const & m_sTexture;
	sf::Texture const & m_nTexture;

	sf::Texture const & m_seSwTexture;
	sf::Texture const & m_neNwTexture;
	sf::Texture const & m_eWTexture;


	KeyHandler const & m_keyhandler;

	XboxController& m_controller;

	Game& m_game;

	Pause *m_pauseScreen;

	//Base movement functions
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void setMaze(MazeGenerator &t_maze);
};

