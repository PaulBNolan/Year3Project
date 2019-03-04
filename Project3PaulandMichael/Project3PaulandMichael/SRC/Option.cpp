/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

#include"Option.h"

Option::Option(Game& t_game, sf::Font t_font, sf::RenderWindow &t_window, KeyHandler const & t_keyHandler, XboxController& t_controller) :
	m_game(t_game), m_font(t_font), m_keyhandler(t_keyHandler), m_controller(t_controller)
{
	m_windowWidth = t_window.getSize().x;
	m_windowHeight = t_window.getSize().y;

	m_activeIndicator = 0;

	m_optionsText[0].setString("Music Volume: ");
	m_optionsText[1].setString("SFX Volume: ");
	m_optionsText[2].setString("Exit");

	for (int i = 0; i < 3; i++)
	{
		m_options[i].setSize(sf::Vector2f(m_windowWidth * 0.6, m_windowHeight * 0.1));
		m_optionsPos[i] = sf::Vector2f(m_windowWidth / 2 - m_options[i].getGlobalBounds().width / 2, m_windowHeight * (0.3 + 0.2 * i) - m_options[i].getGlobalBounds().height / 2);

		m_options[i].setFillColor(sf::Color(4, 104, 255, 255));
		m_options[i].setOutlineColor(sf::Color(166, 0, 255, 255));
		m_options[i].setOutlineThickness(5);
		m_options[i].setPosition(m_optionsPos[i]);

		m_optionsText[i].setFont(m_font);
		m_optionsText[i].setFillColor(sf::Color::Black);
		m_optionsText[i].setCharacterSize(40);
		m_optionsText[i].setPosition(sf::Vector2f((m_windowWidth / 2 - m_optionsText[i].getGlobalBounds().width / 2), m_windowHeight * (0.3 + 0.2 * i) - m_optionsText[i].getGlobalBounds().height / 2));
	}

}

Option::~Option()
{

}

void Option::update(double t)
{
	handleKeyInput(t);
}

void Option::handleKeyInput(double t)
{
	if (!m_analogueStop) // when released check is false, start count the timer and release turn true
	{
		m_timer += t;
		if (m_timer >= 0.2f)
		{
			m_analogueStop = true;
			m_timer = 0;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_options[i].setOutlineColor(sf::Color(166, 0, 255, 255));
		m_options[i].setFillColor(sf::Color(4, 104, 255, 255));
	}

	m_options[m_activeIndicator].setOutlineColor(sf::Color(166, 0, 255, 255));
	m_options[m_activeIndicator].setFillColor(sf::Color(255, 213, 0, 255));
	if ((m_keyhandler.isPressed(sf::Keyboard::Down) && m_downButtonReleased) || (m_controller.m_currentState.DpadDown == true && m_controller.m_previousState.DpadDown != true) || (m_controller.m_previousState.LeftThumbStick.y > 50 && m_analogueStop))
	{
		m_downButtonReleased = false;
		m_activeIndicator++;
		if (m_activeIndicator >= 3)
		{
			m_activeIndicator = 0;
		}
		m_analogueStop = false;
	}
	else if (!m_keyhandler.isPressed(sf::Keyboard::Down))
	{
		m_downButtonReleased = true;
	}

	if ((m_keyhandler.isPressed(sf::Keyboard::Up) && m_upButtonReleased) || (m_controller.m_currentState.DpadUp == true && m_controller.m_previousState.DpadUp != true) || (m_controller.m_currentState.LeftThumbStick.y < -50 && m_analogueStop))
	{
		m_upButtonReleased = false;
		m_activeIndicator--;
		if (m_activeIndicator <= -1)
		{
			m_activeIndicator = 2;

		}
		m_analogueStop = false;
	}
	else if (!m_keyhandler.isPressed(sf::Keyboard::Up))
	{
		m_upButtonReleased = true;
	}

	/// <summary>
	/// when a button pressed, change to other screen
	/// </summary>
	if ((m_keyhandler.isPressed(sf::Keyboard::Enter) && m_enterReleased) || m_controller.m_currentState.A && !m_controller.m_previousState.A)
	{
		m_enterReleased = false;
		switch (m_activeIndicator)
		{
		case 0:
			//m_game.setGameStates(GameStates::Gameplay); // change the game state to gameplay
			break;
		case 1:
			//m_game.setGameStates(GameStates::Tutorial); // change the game state to score board
			break;
		case 2:
			m_game.setGameStates(GameStates::Menu); // change the game state to option
			break;
		default:
			break;
		}
		m_analogueStop = false;
	}
	else if (!m_keyhandler.isPressed(sf::Keyboard::Enter))
	{
		m_enterReleased = true;
	}
}



void Option::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < 4; i++)
	{
		t_window.draw(m_options[i]);
		t_window.draw(m_optionsText[i]);
	}
}
