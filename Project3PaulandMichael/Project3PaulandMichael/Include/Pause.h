#pragma once
#include "SFML\Graphics.hpp"
#include "Game.h"
#include <iostream>
#include "KeyHandler.h"
#include "Player.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

class Game;
class Player;
class XboxController;
class Pause
{
public:
	Pause(Game& t_game, sf::RenderWindow &t_window, KeyHandler const & t_keyHandler, XboxController& t_controller, sf::Font t_font, Player& t_player);
	~Pause();
	void handleKeyInput(double t);
	void update(double t);
	void render(sf::RenderWindow& t_window);
	bool getPaused();
	void setPaused(bool t_paused);
private:
	XboxController & m_controller;
	Game& m_game;
	Player& m_player;

	bool m_analogueStop = true; // check if button release 
	bool m_upButtonReleased = true;
	bool m_downButtonReleased = true;
	bool m_enterReleased = true;
	float m_timer = 0; // release the button 

	KeyHandler const & m_keyhandler;
	float m_windowWidth;
	float m_windowHeight;

	sf::Vector2f isoToCart(float x, float y);
	sf::Vector2f cartToIso(float x, float y);
	bool m_paused = false;

	sf::Font m_font;

	int m_activeIndicator;

	sf::RectangleShape m_options[2];
	sf::Vector2f m_optionsPos[2];
	sf::Text m_optionsText[2];
	sf::Vector2f m_optionsTextPos[2];
	sf::RectangleShape m_background;
};

