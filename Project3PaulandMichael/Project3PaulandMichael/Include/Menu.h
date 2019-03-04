/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

#ifndef MENU
#define MENU
#include "SFML\Graphics.hpp"
#include "Game.h"
#include "XBOXCONTROLLER.h"
#include <iostream>
#include"KeyHandler.h"
class Game;
class XboxController;
class Menu
{
public:
	Menu(Game& t_game, sf::Font t_font, sf::RenderWindow &t_window, KeyHandler const & t_keyHandler, XboxController& t_controller);
	~Menu();

	void handleKeyInput(double t);
	void update(double t);
	void render(sf::RenderWindow& t_window);
private:
	XboxController& m_controller;
	Game& m_game;

	bool m_analogueStop = true; // check if button release 
	bool m_upButtonReleased = true;
	bool m_downButtonReleased = true;
	bool m_enterReleased = true;
	float m_timer = 0; // release the button 

	KeyHandler const & m_keyhandler;
	float m_windowWidth;
	float m_windowHeight;

	sf::Font m_font;

	int m_activeIndicator;

	sf::RectangleShape m_options[4];
	sf::Vector2f m_optionsPos[4];
	sf::Text m_optionsText[4];
	sf::Vector2f m_optionsTextPos[4];
};

#endif