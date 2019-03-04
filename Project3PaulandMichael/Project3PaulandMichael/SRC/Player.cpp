#include "Player.h"
#include "iostream"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

Player::Player(Game& t_game, sf::Texture const & t_texture, sf::Texture const & t_sTexture, sf::Texture const & t_nTexture, sf::Texture const & t_neNwTexture, sf::Texture const & t_seSwTexture, sf::Texture const & t_ewTexture, sf::RenderWindow& t_window, sf::Vector2f t_pos, KeyHandler const & t_keyHandler, XboxController& t_controller, sf::Font t_font)
	: m_game(t_game),m_idleTexture(t_texture),m_sTexture(t_sTexture),m_nTexture(t_nTexture), m_neNwTexture(t_neNwTexture),m_seSwTexture(t_seSwTexture),m_eWTexture(t_ewTexture), m_keyhandler(t_keyHandler) , m_controller(t_controller)
{
	m_pauseScreen = new Pause(t_game, t_window, t_keyHandler, t_controller, t_font, *this);
	initSprites(t_pos);

	m_playerCircle.setFillColor(sf::Color::Red);
	m_playerCircle.setRadius(m_pCircleRadius);
	m_playerCircle.setPosition(sf::Vector2f(m_playerPosition.x - 51.5, m_playerPosition.y));

	m_playerCenter = sf::Vector2f(m_playerPosition.x - 51.5 + m_pCircleRadius, m_playerPosition.y + m_pCircleRadius);

	calculateVelocity();
}


Player::~Player()
{
}

void Player::update(double t, MazeGenerator & t_maze)
{
	handleKeyInput(t_maze);
	m_pauseScreen->update(t);

	m_animationTime += t;

	if (m_animationTime > 1.0 / 8.0)
	{
		animate();
		m_animationTime = 0;
	}
}

void Player::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_playerCircle);
	t_window.draw(m_player);
}

void Player::calculateVelocity()
{
	float direction = atan2f(75.0, 128.0);

	float magnitude = sqrt(pow(128, 2) + pow(75, 2));

	m_velocity = sf::Vector2f(4* magnitude * (cos(direction) / 100.0), 4 * magnitude * (sin(direction) / 100.0));
}

void Player::initSprites(sf::Vector2f const & t_pos)
{
	// Initialise the player
	m_spriteWidth[0] = 80;
	m_spriteHeight[0] = 96;

	m_spriteWidth[1] = 77;
	m_spriteHeight[1] = 101;

	m_spriteWidth[2] = 79;
	m_spriteHeight[2] = 127;

	m_spriteWidth[3] = 80;
	m_spriteHeight[3] = 112;

	m_spriteWidth[4] = 90;
	m_spriteHeight[4] = 127;

	m_spriteWidth[5] = 60;
	m_spriteHeight[5] = 109;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_spriteSheetPositions[i][j] = sf::Vector2f(i * 100, 0);
		}
	}

	m_player.setTexture(m_idleTexture);
	sf::IntRect baseRect(0,0,m_spriteWidth[0],m_spriteHeight[0]);
	m_player.setTextureRect(baseRect);
	m_player.setOrigin((float)(baseRect.width / 2.0), (float)(baseRect.height / 2.0));
	m_player.setScale(sf::Vector2f(1, 2));
	m_playerPosition = t_pos;
	m_player.setPosition(m_playerPosition);
}

void Player::animate()
{
	m_animationStage++;
	if (m_animationStage == 8)
	{
		m_animationStage = 0;
	}

	sf::IntRect baseRect(m_spriteSheetPositions[m_animationStage][m_movementType].x, m_spriteSheetPositions[m_animationStage][m_movementType].y, m_spriteWidth[m_movementType], m_spriteHeight[m_movementType]);
	if (m_player.getScale().x > 0)
	{
		m_player.setOrigin((float)(baseRect.width / 2.0), (float)(baseRect.height / 2.0));
	}
	else
	{
		m_player.setOrigin((float)((baseRect.width / 2.0)), (float)(baseRect.height / 2.0));
	}
	m_player.setTextureRect(baseRect);
}

void Player::moveUp()
{
	m_playerPosition.y -= m_velocity.y;
}

void Player::moveDown()
{
	m_playerPosition.y += m_velocity.y;
}

void Player::moveLeft()
{
	m_playerPosition.x -= m_velocity.x;
}

void Player::moveRight()
{
	m_playerPosition.x += m_velocity.x;
}

void Player::setPosition(sf::Vector2f pos)
{
	if (m_playerPosition != pos)
	{
		m_playerPosition = pos;
	}
	m_player.setPosition(pos);
}

sf::Vector2f Player::getPosition()
{
	return m_player.getPosition();
}

void Player::handleKeyInput(MazeGenerator & t_maze)
{
	if (((m_keyhandler.isPressed(sf::Keyboard::D)) || (m_controller.m_previousState.LeftThumbStick.x > 20)) &&
		(m_keyhandler.isPressed(sf::Keyboard::W) || (m_controller.m_previousState.LeftThumbStick.y < -20)))
	{
		setPDirection(PlayerDirection::NORTHEAST);
		if (m_movementType != 4 || m_player.getScale().x > 0)
		{
			m_movementType = 4;
			m_animationStage = 0;
			m_player.setTexture(m_neNwTexture);
			m_player.setScale(sf::Vector2f(-1, 2));
		}

		moveUp();
		moveRight();
	}

	else if (((m_keyhandler.isPressed(sf::Keyboard::A)) || (m_controller.m_previousState.LeftThumbStick.x < -20)) &&
		(m_keyhandler.isPressed(sf::Keyboard::W) || (m_controller.m_previousState.LeftThumbStick.y < -20)))
	{
		setPDirection(PlayerDirection::NORTHWEST);
		if (m_movementType != 4 || m_player.getScale().x < 0)
		{
			m_movementType = 4;
			m_animationStage = 0;
			m_player.setTexture(m_neNwTexture);
			m_player.setScale(sf::Vector2f(1, 2));
		}

		moveUp();
		moveLeft();
	}

	else if (((m_keyhandler.isPressed(sf::Keyboard::D)) || (m_controller.m_previousState.LeftThumbStick.x > 20)) &&
		(m_keyhandler.isPressed(sf::Keyboard::S) || (m_controller.m_previousState.LeftThumbStick.y > 20)))
	{
		setPDirection(PlayerDirection::SOUTHEAST);
		if (m_movementType != 5 || m_player.getScale().x > 0)
		{
			m_movementType = 5;
			m_animationStage = 0;
			m_player.setTexture(m_seSwTexture);
			m_player.setScale(sf::Vector2f(-1, 2));
		}

		moveDown();
		moveRight();
	}

	else if (((m_keyhandler.isPressed(sf::Keyboard::A)) || (m_controller.m_previousState.LeftThumbStick.x < -20)) &&
		(m_keyhandler.isPressed(sf::Keyboard::S) || (m_controller.m_previousState.LeftThumbStick.y > 20)))
	{
		setPDirection(PlayerDirection::SOUTHWEST);
		if (m_movementType != 5 || m_player.getScale().x < 0)
		{
			m_movementType = 5;
			m_animationStage = 0;
			m_player.setTexture(m_seSwTexture);
			m_player.setScale(sf::Vector2f(1, 2));
		}

		moveDown();
		moveLeft();
	}


	else if ((m_keyhandler.isPressed(sf::Keyboard::D)) || (m_controller.m_previousState.LeftThumbStick.x > 20))
	{
		setPDirection(PlayerDirection::EAST);
	
		if (!m_pauseScreen->getPaused())
		{
			if (m_movementType != 3 || m_player.getScale().x > 0)
			{
				m_movementType = 3;
				m_animationStage = 0;
				m_player.setTexture(m_eWTexture);
				m_player.setScale(sf::Vector2f(-1, 2));
			}
			moveRight();
		}
	}

	else if (m_keyhandler.isPressed(sf::Keyboard::A) || (m_controller.m_previousState.LeftThumbStick.x < -20))
	{
		setPDirection(PlayerDirection::WEST);
	
		if (!m_pauseScreen->getPaused())
		{
			if (m_movementType != 3 || m_player.getScale().x < 0)
			{
				m_movementType = 3;
				m_animationStage = 0;
				m_player.setTexture(m_eWTexture);
				m_player.setScale(sf::Vector2f(1, 2));
			}
			moveLeft();
		}
	}
	else if (m_keyhandler.isPressed(sf::Keyboard::W) || (m_controller.m_previousState.LeftThumbStick.y < -20))
	{
		setPDirection(PlayerDirection::NORTH);
	
		if (!m_pauseScreen->getPaused())
		{
			if (m_movementType != 2)
			{
				m_movementType = 2;
				m_animationStage = 0;
				m_player.setTexture(m_nTexture);
				m_player.setScale(sf::Vector2f(1, 2));
			}
			moveUp();
		}
	}

	else if (m_keyhandler.isPressed(sf::Keyboard::S) || (m_controller.m_previousState.LeftThumbStick.y > 20))
	{
		setPDirection(PlayerDirection::SOUTH);
		
		if (!m_pauseScreen->getPaused())
		{
			if (m_movementType != 1)
			{
				m_movementType = 1;
				m_animationStage = 0;
				m_player.setTexture(m_sTexture);
				m_player.setScale(sf::Vector2f(1, 2));
			}
			moveDown();
		}
	}

	else if (((m_controller.m_previousState.LeftThumbStick.x > -20) && (m_controller.m_previousState.LeftThumbStick.x < 20)) &&
		((m_controller.m_previousState.LeftThumbStick.y > -20) && (m_controller.m_previousState.LeftThumbStick.y < 20)))
	{
		m_movementType = 0;
		m_animationStage = 0;
		m_player.setTexture(m_idleTexture);
	}

	if (m_keyhandler.isPressed(sf::Keyboard::P) || (m_controller.m_currentState.Start && !m_controller.m_previousState.Start))
	{
		m_pauseScreen->setPaused(true);
	}

	if (m_keyhandler.isPressed(sf::Keyboard::Space) || m_controller.m_currentState.A == true)
	{
		t_maze.breakWall();
	}

	m_player.setPosition(m_playerPosition);
	m_playerCircle.setPosition(sf::Vector2f(m_playerPosition.x - 56.5, m_playerPosition.y));
	m_playerCenter = sf::Vector2f(m_playerPosition.x - 56.5 + m_pCircleRadius, m_playerPosition.y + m_pCircleRadius);
}

sf::Sprite Player::getPlayer() const
{
	return m_player;
}

bool Player::getPaused()
{
	return m_pauseScreen->getPaused();
}

void Player::renderPauseScreen(sf::RenderWindow & t_window)
{
	m_pauseScreen->render(t_window);
}

void Player::collisionCheck(sf::CircleShape t_wCircle,sf::Vector2f t_wCircleCenter)
{
	if (sqrt((t_wCircleCenter.x - m_playerCenter.x) * (t_wCircleCenter.x - m_playerCenter.x) + (t_wCircleCenter.y - m_playerCenter.y) * 
		(t_wCircleCenter.y - m_playerCenter.y)) < t_wCircle.getRadius() + m_pCircleRadius)
	{
		if (t_wCircleCenter.x < m_playerCenter.x)
		{
			m_playerPosition.x += m_velocity.x;
		}
		else
		{
			m_playerPosition.x -= m_velocity.x;
		}

		if (t_wCircleCenter.y < m_playerCenter.y)
		{
			m_playerPosition.y += m_velocity.y;
		}
		else
		{
			m_playerPosition.y -= m_velocity.x;
		}
	}
}

void Player::setMaze(MazeGenerator &t_maze) 
{
	//m_maze = t_maze;
}

void Player::setPDirection(PlayerDirection t_newDirection)
{
	m_playerDirection = t_newDirection;
}