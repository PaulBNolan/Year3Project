#include "Enemy.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

Enemy::Enemy(Game& t_game, sf::Texture const & t_texture,sf::Texture const & t_SEtexture, sf::Texture const & t_NWtexture, sf::Texture const & t_SERtexture, sf::Texture const & t_NWRtexture):
	m_game(t_game),m_texture(t_texture),m_seTexture(t_SEtexture),m_nwTexture(t_NWtexture),m_seRunningTexture(t_SERtexture),m_nwRunningTexture(t_NWRtexture)
{
	m_enemyCircle.setRadius(m_eCircleRadius);
	m_enemyCircle.setFillColor(sf::Color::Cyan);
	calculateVelocity();
	initSprites();
}


Enemy::~Enemy()
{
}

void Enemy::initSprites()
{
	m_spriteWidth[0] = 73.0;
	m_spriteHeight[0] = 79.0;

	m_spriteWidth[1] = 87.0;
	m_spriteHeight[1] = 82.0;

	m_spriteWidth[2] = 73.0;
	m_spriteHeight[2] = 94.0;

	m_spriteWidth[3] = 96.0;
	m_spriteHeight[3] = 78.0;

	m_spriteWidth[4] = 80.0;
	m_spriteHeight[4] = 98.0;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_spriteSheetPositions[i][j] = sf::Vector2f(100 * i, 0);
		}
	}

	sf::IntRect baseRect(m_spriteSheetPositions[m_animationStage][m_movementType].x, m_spriteSheetPositions[m_animationStage][m_movementType].y, m_spriteWidth[m_movementType], m_spriteHeight[m_movementType]);
	m_enemy.setTexture(m_texture);
	m_enemy.setScale(sf::Vector2f(1, 2));
	m_enemy.setTextureRect(baseRect);
}

void Enemy::calculateVelocity()
{
	float direction = atan2f(75.0, 128.0);

	float magnitude = sqrt(pow(128, 2) + pow(75, 2));

	m_velocity = sf::Vector2f( 2 * magnitude * (cos(direction) / 100.0), 2 * magnitude * (sin(direction) / 100.0));
}


void Enemy::update(double t_time,MazeGenerator & t_maze)
{
	m_animationTime += t_time;
	movement(t_maze);
	breakWalls(t_maze);

	if (t_maze.m_highestPTrack > 20 && m_running == false)
	{
		startRunning(t_maze);
	}

	if (m_animationTime > 1.0 / 6.0)
	{
		animate();
		m_animationTime = 0;
	}
}

void Enemy::render(sf::RenderWindow & window)
{
	window.draw(m_enemy);
}

void Enemy::setPosition(sf::Vector2f pos)
{
	m_enemyPosition = pos;
	m_enemyCircle.setPosition(pos);
	m_enemy.setPosition(pos);
}

sf::Vector2f Enemy::getPosition()
{
	return m_enemyPosition;
}

void Enemy::animate()
{
	m_animationStage++;
	if (m_animationStage == 6)
	{
		m_animationStage = 0;
	}

	sf::IntRect baseRect(m_spriteSheetPositions[m_animationStage][m_movementType].x, m_spriteSheetPositions[m_animationStage][m_movementType].y, m_spriteWidth[m_movementType], m_spriteHeight[m_movementType]);
	m_enemy.setTextureRect(baseRect);
}

bool Enemy::collisionCheck(sf::CircleShape t_wCircle, sf::Vector2f t_wCircleCenter)
{
	if (sqrt((t_wCircleCenter.x - m_enemyCenter.x) * (t_wCircleCenter.x - m_enemyCenter.x) + (t_wCircleCenter.y - m_enemyCenter.y) *
		(t_wCircleCenter.y - m_enemyCenter.y)) < t_wCircle.getRadius() + m_eCircleRadius)
	{
		return true;
	}

	else
	{
		return false;
	}
}

void Enemy::movement(MazeGenerator & t_maze)
{
	switch (m_eDirection)
	{
	case(EnemyDirection::NORTH):
		switch (m_running)
		{
		case(false):
			m_enemyPosition.x += m_velocity.x;
			m_enemyPosition.y -= m_velocity.y;
			break;
		case(true):
			m_enemyPosition.x += m_velocity.x * 2;
			m_enemyPosition.y -= m_velocity.y * 2;
			break;
		}
		break;
	case(EnemyDirection::SOUTH):
		switch (m_running)
		{
		case(false):
			m_enemyPosition.y += m_velocity.y;
			m_enemyPosition.x -= m_velocity.x;
			break;
		case(true):
			m_enemyPosition.y += m_velocity.y * 2;
			m_enemyPosition.x -= m_velocity.x * 2;
			break;
		}
		break;

	case(EnemyDirection::WEST):
		switch (m_running)
		{
		case(false):
			m_enemyPosition -= m_velocity;
			break;
		case(true):
			m_enemyPosition -= sf::Vector2f(m_velocity.x * 2, m_velocity.y * 2);
			break;
		}
		break;

	case(EnemyDirection::EAST):
		switch (m_running)
		{
		case(false):
			m_enemyPosition += m_velocity;
			break;
		case(true):
			m_enemyPosition += sf::Vector2f(m_velocity.x * 2, m_velocity.y * 2);
			break;
		}
		break;
	}

	m_enemyCircle.setPosition(m_enemyPosition);
	m_enemyCenter = sf::Vector2f(m_enemyPosition.x + m_eCircleRadius, m_enemyPosition.y + m_eCircleRadius);

	if (m_enemy.getScale().x > 0)
	{
		m_enemyTexturePosition = sf::Vector2f(m_enemyPosition.x, m_enemyPosition.y - m_enemy.getGlobalBounds().height);
	}
	else
	{
		m_enemyTexturePosition = sf::Vector2f(m_enemyPosition.x + m_enemy.getGlobalBounds().width, m_enemyPosition.y - m_enemy.getGlobalBounds().height);
	}

	m_enemy.setPosition(m_enemyTexturePosition);

	enemyTurn(t_maze);

	if ((((m_eDirection == EnemyDirection::NORTH && t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == true)|| (m_eDirection == EnemyDirection::SOUTH && t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == true))
		&& (t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == true || t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == true))
		|| (((m_eDirection == EnemyDirection::WEST && t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == true) || (m_eDirection == EnemyDirection::EAST && t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == true))
		&& (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == true || t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == true)))
	{
		checkForPlayerTracks(t_maze);
	}
}

void Enemy::enemyTurn(MazeGenerator & t_maze)
{
	srand((unsigned)time(NULL));
	if ((m_eDirection == EnemyDirection::NORTH && t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == false)
		|| (m_eDirection == EnemyDirection::SOUTH && t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == false))
	{
		if (t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == true
			&&
			t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == true)
		{
			int turn = rand() % 2;

			if (turn == 0)
			{
				changeDirection(EnemyDirection::EAST);
			}
			else if (turn == 1)
			{
				changeDirection(EnemyDirection::WEST);
			}
		}

		else if (t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == true
			&&
			t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == false)
		{
			changeDirection(EnemyDirection::EAST);
		}

		else if (t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == false
			&&
			t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == true)
		{
			changeDirection(EnemyDirection::WEST);
		}

		else if(m_eDirection == EnemyDirection::NORTH)
		{
			changeDirection(EnemyDirection::SOUTH);
		}

		else if (m_eDirection == EnemyDirection::SOUTH)
		{
			changeDirection(EnemyDirection::NORTH);
		}
	}

	else if ((m_eDirection == EnemyDirection::WEST && t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == false) 
		|| (m_eDirection == EnemyDirection::EAST && t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == false))
	{
		if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == true
			&&
			t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == true)
		{
			int turn = rand() % 2;

			if (turn == 0)
			{
				changeDirection(EnemyDirection::NORTH);
			}
			else if (turn == 1)
			{
				changeDirection(EnemyDirection::SOUTH);
			}
		}

		else if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == true
			&&
			t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == false)
		{
			changeDirection(EnemyDirection::SOUTH);
		}

		else if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == false
			&&
			t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == true)
		{
			changeDirection(EnemyDirection::NORTH);
		}

		else if(m_eDirection == EnemyDirection::WEST)
		{
			changeDirection(EnemyDirection::EAST);
		}

		else if (m_eDirection == EnemyDirection::EAST)
		{
			changeDirection(EnemyDirection::WEST);
		}
	}

}

void Enemy::checkForPlayerTracks(MazeGenerator & t_maze)
{
	int m_northCurrent = 0;
	int m_northTrack = 0;

	int m_southCurrent = 0;
	int m_southTrack = 0;

	int m_eastCurrent = 0;
	int m_eastTrack = 0;

	int m_westCurrent = 0;
	int m_westTrack = 0;

	if (t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath == true)
	{
		m_westCurrent = t_maze.getEnemyTile().x - 1;

		while (t_maze.m_map[m_westCurrent][t_maze.getEnemyTile().y].isPath == true)
		{
			if (t_maze.m_map[m_westCurrent][t_maze.getEnemyTile().y].m_playerTracks > m_westTrack)
			{
				m_westTrack = t_maze.m_map[m_westCurrent][t_maze.getEnemyTile().y].m_playerTracks;
			}
			m_westCurrent--;
		}
	}

	if (t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath == true)
	{
		m_eastCurrent = t_maze.getEnemyTile().x + 1;

		while (t_maze.m_map[m_eastCurrent][t_maze.getEnemyTile().y].isPath == true)
		{
			if (t_maze.m_map[m_eastCurrent][t_maze.getEnemyTile().y].m_playerTracks > m_eastTrack)
			{
				m_eastTrack = t_maze.m_map[m_eastCurrent][t_maze.getEnemyTile().y].m_playerTracks;
			}
			m_eastCurrent++;
		}
	}

	if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath == true)
	{
		m_northCurrent = t_maze.getEnemyTile().y - 1;

		while (t_maze.m_map[t_maze.getEnemyTile().x][m_northCurrent].isPath == true)
		{
			if (t_maze.m_map[t_maze.getEnemyTile().x][m_northCurrent].m_playerTracks > m_northTrack)
			{
				m_northTrack = t_maze.m_map[t_maze.getEnemyTile().x][m_northCurrent].m_playerTracks;
			}
			m_northCurrent--;
		}
	}

	if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath == true)
	{
		m_southCurrent = t_maze.getEnemyTile().y + 1;

		while (t_maze.m_map[t_maze.getEnemyTile().x][m_southCurrent].isPath == true)
		{
			if (t_maze.m_map[t_maze.getEnemyTile().x][m_southCurrent].m_playerTracks > m_northTrack)
			{
				m_southTrack = t_maze.m_map[t_maze.getEnemyTile().x][m_southCurrent].m_playerTracks;
			}
			m_southCurrent++;
		}
	}

	//This is used to find the max of the four values
	int m_targetPath = std::max(std::max(m_northTrack,m_southTrack), std::max(m_eastTrack,m_westTrack));
	if (m_targetPath != 0)
	{
		if (m_targetPath == m_northTrack)
		{
			changeDirection(EnemyDirection::NORTH);
		}

		else if (m_targetPath == m_southTrack)
		{
			changeDirection(EnemyDirection::SOUTH);
		}

		else if (m_targetPath == m_eastTrack)
		{
			changeDirection(EnemyDirection::EAST);
		}

		else if (m_targetPath == m_westTrack)
		{
			changeDirection(EnemyDirection::WEST);
		}
	}

}

void Enemy::startRunning(MazeGenerator & t_maze)
{
	int i = 0;
	bool m_found = false;
	switch (m_eDirection)
	{
	case(EnemyDirection::NORTH):
		while (i < 10 && t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - i].m_playerTracks < t_maze.m_highestPTrack - 10
			&&
			t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - i].isPath == true)
		{
			i++;
			if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - i].m_playerTracks > t_maze.m_highestPTrack - 10)
			{
				m_found = true;
			}
		}
		break;
	case(EnemyDirection::SOUTH):
		while (i < 10 && t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + i].m_playerTracks < t_maze.m_highestPTrack - 10
			&&
			t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + i].isPath == true)
		{
			i++;
			if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - i].m_playerTracks > t_maze.m_highestPTrack - 10)
			{
				m_found = true;
			}
		}
		break;
	case(EnemyDirection::EAST):
		while (i < 10 && t_maze.m_map[t_maze.getEnemyTile().x + i][t_maze.getEnemyTile().y].m_playerTracks < t_maze.m_highestPTrack - 10
			&&
			t_maze.m_map[t_maze.getEnemyTile().x + i][t_maze.getEnemyTile().y].isPath == true)
		{
			i++;
			if (t_maze.m_map[t_maze.getEnemyTile().x + i][t_maze.getEnemyTile().y].m_playerTracks > t_maze.m_highestPTrack - 10)
			{
				m_found = true;
			}
		}
		break;
	case(EnemyDirection::WEST):
		while (i < 10 && t_maze.m_map[t_maze.getEnemyTile().x - i][t_maze.getEnemyTile().y].m_playerTracks < t_maze.m_highestPTrack - 10
			&&
			t_maze.m_map[t_maze.getEnemyTile().x - i][t_maze.getEnemyTile().y].isPath == true)
		{
			i++;
			if (t_maze.m_map[t_maze.getEnemyTile().x - i][t_maze.getEnemyTile().y].m_playerTracks > t_maze.m_highestPTrack - 10)
			{
				m_found = true;
			}
		}
		break;
	}

	if (m_found == true)
	{
		m_running = true;
		m_movementType += 2;
	}
}

void Enemy::breakWalls(MazeGenerator & t_maze)
{
	if (t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].breakable == true
		&&
		t_maze.m_map[t_maze.getEnemyTile().x - 2][t_maze.getEnemyTile().y].m_playerPath == true)
	{
		t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].bottomWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].topWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].leftWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].rightWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x - 1][t_maze.getEnemyTile().y].isPath = true;

		changeDirection(EnemyDirection::WEST);
	}

	else if (t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].breakable == true
		&&
		t_maze.m_map[t_maze.getEnemyTile().x + 2][t_maze.getEnemyTile().y].m_playerPath == true)
	{
		t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].bottomWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].topWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].leftWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].rightWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x + 1][t_maze.getEnemyTile().y].isPath = true;

		changeDirection(EnemyDirection::EAST);
	}

	if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].breakable == true
		&&
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 2].m_playerPath == true)
	{
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].bottomWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].topWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].leftWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].rightWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y - 1].isPath = true;

		changeDirection(EnemyDirection::NORTH);
	}

	if (t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].breakable == true
		&&
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 2].m_playerPath == true)
	{
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].bottomWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].topWall = false;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].leftWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].rightWall = true;
		t_maze.m_map[t_maze.getEnemyTile().x][t_maze.getEnemyTile().y + 1].isPath = true;

		changeDirection(EnemyDirection::SOUTH);
	}
}

void Enemy::changeDirection(EnemyDirection t_direction)
{
	m_eDirection = t_direction;
	switch(m_eDirection)
	{
	case(EnemyDirection::NORTH):
		switch (m_running)
		{
		case(false):
			m_movementType = 2;
			m_enemy.setTexture(m_nwTexture);
			break;
		case(true):
			m_movementType = 4;
			m_enemy.setTexture(m_nwRunningTexture);
			break;
		}
		m_animationStage = 5;
		m_enemy.setScale(sf::Vector2f(-1, 2));
		break;
	case(EnemyDirection::SOUTH):
		switch (m_running)
		{
		case(false):
			m_movementType = 1;
			m_enemy.setTexture(m_seTexture);
			break;
		case(true):
			m_movementType = 3;
			m_enemy.setTexture(m_seRunningTexture);
			break;
		}
		m_animationStage = 5;
		m_enemy.setScale(sf::Vector2f(1, 2));
		break;

	case(EnemyDirection::WEST):
		switch (m_running)
		{
		case(false):
			m_movementType = 2;
			m_enemy.setTexture(m_nwTexture);
			break;
		case(true):
			m_movementType = 4;
			m_enemy.setTexture(m_nwRunningTexture);
			break;
		}
		m_animationStage = 5;
		m_enemy.setScale(sf::Vector2f(1, 2));
		break;

	case(EnemyDirection::EAST):
		switch (m_running)
		{
		case(false):
			m_movementType = 1;
			m_enemy.setTexture(m_seTexture);
			break;
		case(true):
			m_movementType = 3;
			m_enemy.setTexture(m_seRunningTexture);
			break;
		}
		m_animationStage = 5;
		m_enemy.setScale(sf::Vector2f(-1, 2));
		break;
	}
	animate();
}