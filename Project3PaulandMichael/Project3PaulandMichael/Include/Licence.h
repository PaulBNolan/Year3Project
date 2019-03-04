/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan,
/// @version 1.0
/// </summary>
#pragma once
#include "SFML\Graphics.hpp"
#include "Game.h"
#include <iostream>
#include "KeyHandler.h"
class Game;
class XboxController;
class Licence
{
public:
	Licence(Game& t_game, sf::Font t_font, sf::RenderWindow &t_window, KeyHandler const & t_keyHandler, XboxController& t_controller);
	~Licence();
	void update(double t);
	void handleKeyInput();
	void render(sf::RenderWindow& t_window);
private:

	void setUpSprite();
	XboxController& m_controller;
	Game& m_game;

	float m_windowWidth;
	float m_windowHeight;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Font m_font;
	KeyHandler const & m_keyhandler;

	sf::Text m_welcomeText;
	sf::Text m_pressStart;
	sf::Vector2f m_welcomeTextPosition;
	sf::Vector2f m_pressStartPosition;
};

