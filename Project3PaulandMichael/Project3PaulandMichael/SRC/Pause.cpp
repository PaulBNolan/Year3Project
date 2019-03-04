#include "Pause.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

Pause::Pause(Game& t_game, sf::RenderWindow &t_window, KeyHandler const & t_keyHandler, XboxController& t_controller, sf::Font t_font, Player& t_player):
	m_game(t_game), m_keyhandler(t_keyHandler), m_controller(t_controller), m_font(t_font), m_player(t_player)
{
	m_windowWidth = t_window.getSize().x;
	m_windowHeight = t_window.getSize().y;

	m_activeIndicator = 0;

	m_optionsText[0].setString("Resume");
	m_optionsText[1].setString("Quit");

	for (int i = 0; i < 2; i++)
	{
		m_options[i].setSize(sf::Vector2f(m_windowWidth * 0.6, m_windowHeight * 0.1));
		m_options[i].setScale(1.0f, 2.0f);
		m_optionsPos[i] = sf::Vector2f(m_player.getPosition().x, m_player.getPosition().y * (i + 0.3));

		m_options[i].setFillColor(sf::Color(4, 104, 255, 255));
		m_options[i].setOutlineColor(sf::Color(166, 0, 255, 255));
		m_options[i].setOutlineThickness(5);
		m_options[i].setPosition(m_optionsPos[i]);

		m_optionsText[i].setFont(m_font);
		m_optionsText[i].setFillColor(sf::Color::Black);
		m_optionsText[i].setCharacterSize(40);
		m_optionsText[i].setScale(1.0f, 2.0f);
		m_optionsText[i].setPosition(sf::Vector2f(m_player.getPosition().x, m_player.getPosition().y * i));
	}

	m_background.setSize(sf::Vector2f(m_windowWidth * 0.8, m_windowHeight * 0.6));
	m_background.setFillColor(sf::Color(76, 70, 50, 2));
	m_background.setScale(1.0f, 2.0f);
	m_background.setPosition(sf::Vector2f(sf::Vector2f(m_player.getPosition().x - m_background.getGlobalBounds().width / 2, m_player.getPosition().y + m_background.getGlobalBounds().height / 2)));
}


Pause::~Pause()
{
}

void Pause::handleKeyInput(double t)
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

	for (int i = 0; i < 2; i++)
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
		if (m_activeIndicator >= 2)
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
			m_activeIndicator = 1;
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
			m_paused = false;
			break;
		case 1:
			m_paused = false;
			m_game.setGameStates(GameStates::Menu); // change the game state to menu
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

	if (m_keyhandler.isPressed(sf::Keyboard::Escape) || m_controller.m_currentState.B && !m_controller.m_previousState.B)
	{
		m_paused = false;
		m_analogueStop = false;
	}
}

void Pause::update(double t)
{
	m_background.setPosition(sf::Vector2f(m_player.getPosition().x - m_background.getGlobalBounds().width / 2, m_player.getPosition().y + 105 - m_background.getGlobalBounds().height / 2));
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			m_optionsPos[i] = sf::Vector2f(m_player.getPosition().x - m_options[i].getGlobalBounds().width / 2, m_background.getPosition().y + m_options[i].getGlobalBounds().height * 1.25);
		}
		else
		{
			m_optionsPos[i] = sf::Vector2f(m_player.getPosition().x - m_options[i].getGlobalBounds().width / 2, m_background.getPosition().y + m_options[i].getGlobalBounds().height * 3.75);
		}
		m_options[i].setPosition(m_optionsPos[i]);
		m_optionsText[i].setPosition(sf::Vector2f((m_options[i].getPosition().x + (m_options[i].getGlobalBounds().width/ 2 - m_optionsText[i].getGlobalBounds().width / 2)), m_options[i].getPosition().y + (m_options[i].getGlobalBounds().height / 2 - m_optionsText[i].getGlobalBounds().height / 2)));
	}
	if (m_paused)
	{
		handleKeyInput(t);
	}
}

void Pause::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_background);
	for (int i = 0; i < 2; i++)
	{
		t_window.draw(m_options[i]);
		t_window.draw(m_optionsText[i]);
	}
}

bool Pause::getPaused()
{
	return m_paused;
}

void Pause::setPaused(bool t_paused)
{
	m_paused = t_paused;
}

sf::Vector2f Pause::isoToCart(float x, float y)
{
	sf::Vector2f cart;
	cart.x = (2 * y + x) / 2;
	cart.y = (2 * y - x) / 2;
	return cart;
}

sf::Vector2f Pause::cartToIso(float x, float y)
{
	sf::Vector2f iso;
	iso.x = x - y;
	iso.y = (x + y) / 2;
	return iso;
}
