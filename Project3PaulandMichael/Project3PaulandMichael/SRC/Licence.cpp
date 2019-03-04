/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

#include "Licence.h"

Licence::Licence(Game & t_game, sf::Font t_font, sf::RenderWindow & t_window, KeyHandler const & t_keyHandler, XboxController & t_controller) :
	m_game(t_game), m_font(t_font), m_keyhandler(t_keyHandler), m_controller(t_controller)
{
	setUpSprite();
	m_welcomeText.setFont(m_font); // set font to text
	m_welcomeText.setCharacterSize(180);
	m_welcomeText.setFillColor(sf::Color::Yellow); // set colour for text
	m_welcomeText.setOutlineThickness(2); // set the out line's length
	m_welcomeText.setOutlineColor(sf::Color::Black); // set the outline colour
	m_welcomeText.setString("The a-MAZE-ing game!");
	m_pressStart.setFont(m_font); // set font to text
	m_pressStart.setFillColor(sf::Color::Yellow); // set colour for text
	m_pressStart.setOutlineThickness(2); // set the out line's length
	m_pressStart.setOutlineColor(sf::Color::Black); // set the outline colour
	m_pressStart.setString("Press ''Space'' or ''Start'' to continue");
	m_welcomeTextPosition.x = t_window.getSize().x / 2 - m_welcomeText.getGlobalBounds().width / 2;
	m_welcomeTextPosition.y = 50;
	m_pressStartPosition.x = t_window.getSize().x / 2 - m_pressStart.getGlobalBounds().width / 2;
	m_pressStartPosition.y = 500;
	m_pressStartPosition.x = t_window.getSize().x / 2 - m_pressStart.getGlobalBounds().width / 2;
	m_welcomeText.setPosition(m_welcomeTextPosition);
	m_pressStart.setPosition(m_pressStartPosition);
}

Licence::~Licence()
{
}

void Licence::update(double t)
{
	handleKeyInput();
}

void Licence::handleKeyInput()
{
	if ((m_keyhandler.isPressed(sf::Keyboard::Space)) || (m_controller.m_currentState.Start && !m_controller.m_previousState.Start))
	{
		m_game.setGameStates(GameStates::Menu);
	}
}

void Licence::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_sprite);
	t_window.draw(m_welcomeText);
	t_window.draw(m_pressStart);
}

void Licence::setUpSprite()
{
	if (!m_texture.loadFromFile("./Resources/Images/Licence/crop.jfif"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(0, 0);
	m_sprite.setScale(1.5, 1.5);
}
