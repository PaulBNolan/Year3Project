#include "MazeGenerator.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

MazeGenerator::MazeGenerator(Game& t_game, Player& t_player, Enemy& t_enemy)
	: m_game(t_game), m_player(t_player), m_enemy(t_enemy)
{
	for (int row = 0; row < MAZESIZE; row++)
	{
		
		for (int col = 0; col < MAZESIZE; col++)
		{

			int v1 = rand() % 100;
			if (v1 % 5 == 0)
			{
				m_maze[row][col] = true;
			}
			else
			{
				m_maze[row][col] = false;
			}
			
		}
		
	}
	for (int i = 0; i < MAZESIZE; i++)
	{
		for (int j = 0; j < MAZESIZE; j++)
		{
			if (i == 0 && j == 0)
			{
				m_groundPosition[i][j].x = 0;
				m_groundPosition[i][j].y = 0;
			}
			else if (i == 0 && j > 0)
			{
				m_groundPosition[i][j].x = m_groundPosition[i][j - 1].x - 128;
				m_groundPosition[i][j].y = m_groundPosition[i][j - 1].y + 75;
			}
			else
			{
				m_groundPosition[i][j].x = m_groundPosition[i - 1][j].x + 128;
				m_groundPosition[i][j].y = m_groundPosition[i - 1][j].y + 75;
			}

			m_wallCircles[i][j].setFillColor(sf::Color::Blue);
			m_wallCircles[i][j].setRadius(m_wCircleRadius);
			m_wallCircles[i][j].setPosition(sf::Vector2f(m_groundPosition[i][j].x - 64, m_groundPosition[i][j].y));

			m_wCirclesCenter[i][j] = sf::Vector2f(m_groundPosition[i][j].x - 64.0f + m_wCircleRadius, m_groundPosition[i][j].y + m_wCircleRadius);
		}
	}
	Initialize();
}


MazeGenerator::~MazeGenerator()
{
}

void MazeGenerator::Initialize()
{
	for (int i = 0; i < MAZESIZE; i++) 
	{
		for (int j = 0; j < MAZESIZE; j++) 
		{
			m_map[i][j].isPath = false;
			m_map[i][j].visited = false;
			m_map[i][j].topWall = true;
			m_map[i][j].bottomWall = true;
			m_map[i][j].leftWall = true;
			m_map[i][j].rightWall = true;

			m_map[i][j].m_playerPath = false;
		}
	}
	for (int i = 1; i < MAZESIZE - 1; i++) 
	{
		for (int j = 1; j < MAZESIZE - 1; j++)
		{
			// Border Cells have fewer accessible walls
			m_map[1][j].leftWall = false;
			m_map[MAZESIZE - 2][j].rightWall = false;
			m_map[i][1].topWall = false;
			m_map[i][MAZESIZE - 2].bottomWall = false;
		}
	}

	GenerateMaze();
	cleanMaze();

	createIslands();
	createBreakableWalls();

	for (int i = 0; i < MAZESIZE; i++)
	{
		for (int j = 0; j < MAZESIZE; j++)
		{
			// Border Cells have fewer accessible walls
			m_map[MAZESIZE - 1][j].topWall = false;
			m_map[MAZESIZE - 1][j].bottomWall = false;
			//m_map[i][0].topWall = false;
			m_map[i][MAZESIZE - 1].leftWall = false;
			m_map[i][MAZESIZE - 1].rightWall = false;
		}
	}
	m_highestPTrack = 0;
	setEnemy();
}

// GENERATE MAZE
void MazeGenerator::GenerateMaze()
{
	srand((unsigned)time(NULL));                                                           // Pick random start cell
	int randomDirection = 0;
	int rX = (2 * rand() + 1) % (MAZESIZE - 1);                                          // Generate a random odd number between 1 and SIZE
	int rY = (2 * rand() + 1) % (MAZESIZE - 1);   
	int visitedCells = 1;
	int totalCells = ((MAZESIZE - 1) / 2)*((MAZESIZE - 1) / 2);
	int percent = 0;
	m_map[rX][rY].visited = true; // Set start cell as visited;
	m_map[rX][rY].isPath = true;
	while (visitedCells < totalCells)
	{
		if (((m_map[rX - 2][rY].visited == false) && (m_map[rX][rY].leftWall == true && m_map[rX - 2][rY].rightWall == true)) ||
			((m_map[rX + 2][rY].visited == false) && (m_map[rX][rY].rightWall == true && m_map[rX + 2][rY].leftWall == true)) ||
			((m_map[rX][rY - 2].visited == false) && (m_map[rX][rY].topWall == true && m_map[rX][rY - 2].bottomWall == true)) ||
			((m_map[rX][rY + 2].visited == false) && (m_map[rX][rY].bottomWall == true && m_map[rX][rY + 2].topWall == true)))
		{
			randomDirection = (rand() % 4);  // Pick a random wall 1-4 to knock down
			if (randomDirection == 0)
			{
				m_game.setDirection(WallDirection::NORTH);
			}
			else if (randomDirection == 1)
			{
				m_game.setDirection(WallDirection::SOUTH);
			}
			else if (randomDirection == 2)
			{
				m_game.setDirection(WallDirection::EAST);
			}
			else if (randomDirection == 3)
			{
				m_game.setDirection(WallDirection::WEST);
			}
			switch (m_game.getDirection())
			{
			case(WallDirection::WEST):
				if (rX > 1)
				{
					if (m_map[rX - 2][rY].visited == false)
					{											// If not visited
						m_map[rX - 1][rY].isPath = true;        // Mark cell as path
						m_map[rX - 1][rY].visited = true;       // Mark cell as visited
						m_map[rX][rY].leftWall = false;			// Knock down wall

						m_backTrackX.push(rX);                   // Push X for back track
						m_backTrackY.push(rY);                   // Push Y for back track

						rX -= 2;                                // Move to next cell
						m_map[rX][rY].visited = true;			// Mark cell moved to as visited
						m_map[rX][rY].isPath = true;			// Update path
						m_map[rX][rY].rightWall = false;			// Knock down wall
						visitedCells++;							// Increase visitedCells counter
					}
				}
				break;
			case(WallDirection::EAST):
				if (rX < MAZESIZE - 2)
				{
					if (m_map[rX + 2][rY].visited == false)
					{											// If not visited
						m_map[rX + 1][rY].isPath = true;        // Mark cell as path
						m_map[rX + 1][rY].visited = true;       // Mark cell as visited
						m_map[rX][rY].rightWall = false;		// Knock down wall

						m_backTrackX.push(rX);                   // Push X for back track
						m_backTrackY.push(rY);                   // Push Y for back track

						rX += 2;                                // Move to next cell
						m_map[rX][rY].visited = true;			// Mark cell moved to as visited
						m_map[rX][rY].isPath = true;			// Update path
						m_map[rX][rY].leftWall = false;			// Knock down wall
						visitedCells++;                         // Increase visitedCells counter
					}
				}
				break;
			case(WallDirection::NORTH):
				if (rY > 1)
				{
					if (m_map[rX][rY - 2].visited == false)
					{											// If not visited
						m_map[rX][rY - 1].isPath = true;        // Mark cell as path
						m_map[rX][rY - 1].visited = true;       // Mark cell as visited
						m_map[rX][rY].topWall = false;			// Knock down wall

						m_backTrackX.push(rX);                   // Push X for back track
						m_backTrackY.push(rY);                   // Push Y for back track

						rY -= 2;                                // Move to next cell
						m_map[rX][rY].visited = true;			// Mark cell moved to as visited
						m_map[rX][rY].isPath = true;			// Update path
						m_map[rX][rY].bottomWall = false;		// Knock down wall
						visitedCells++;                         // Increase visitedCells counter
					}
				}
				break;
			case(WallDirection::SOUTH):
				if (rY < MAZESIZE - 2)
				{
					if (m_map[rX][rY + 2].visited == false)
					{											// If not visited
						m_map[rX][rY + 1].isPath = true;        // Mark cell as path
						m_map[rX][rY + 1].visited = true;       // Mark cell as visited
						m_map[rX][rY].bottomWall = false;		// Knock down wall

						m_backTrackX.push(rX);                   // Push X for back track
						m_backTrackY.push(rY);                   // Push Y for back track

						rY += 2;                                // Move to next cell
						m_map[rX][rY].visited = true;			// Mark cell moved to as visited
						m_map[rX][rY].isPath = true;			// Update path
						m_map[rX][rY].topWall = false;			// Knock down wall
						visitedCells++;                         // Increase visitedCells counter
					}
				}
				break;
			}
			percent = (visitedCells * 100 / totalCells * 100) / 100;                // Progress in percentage
			std::cout << std::endl << "       Generating a Random Maze... " << percent << "%" << std::endl;
			std::cout << "X: " << rX << " Y: " << rY << std::endl;
		}
		else
		{
			rX = m_backTrackX.top();
			m_backTrackX.pop();

			rY = m_backTrackY.top();
			m_backTrackY.pop();
		}
	}


	//Entrance for main level 1 or Exit for main level 2 is set
	randomDirection = rand() % 4;
	if (randomDirection == 0)
	{
		m_game.setDirection(WallDirection::NORTH);
	}
	else if (randomDirection == 1)
	{
		m_game.setDirection(WallDirection::SOUTH);
	}
	else if (randomDirection == 2)
	{
		m_game.setDirection(WallDirection::EAST);
	}
	else if (randomDirection == 3)
	{
		m_game.setDirection(WallDirection::WEST);
	}
	int sP = rand() % (MAZESIZE - 2) +1;

	switch (m_game.getDirection())
	{
		case(WallDirection::NORTH):
			while (m_map[1][sP].isPath == false)
			{
				sP = rand() % (MAZESIZE - 2) + 1;
			}
			m_map[0][sP].isPath = true;
			m_startPosition = m_groundPosition[0][sP];
			m_startRow = 0;
			m_startColumn = sP;
			m_player.setPDirection(PlayerDirection::SOUTH);
			m_pX = 0;
			m_pY = sP;
			break;
		case(WallDirection::SOUTH):
			while (m_map[MAZESIZE - 2][sP].isPath == false)
			{
				sP = rand() % (MAZESIZE - 2) + 1;
			}
			m_map[MAZESIZE - 1][sP].isPath = true;
			m_startPosition = m_groundPosition[MAZESIZE - 1][sP];
			m_startRow = MAZESIZE - 1;
			m_startColumn = sP;
			m_player.setPDirection(PlayerDirection::NORTH);
			m_pX = MAZESIZE - 1;
			m_pY = sP;
			break;
		case(WallDirection::EAST):
			while (m_map[sP][0].isPath == false)
			{
				sP = rand() % (MAZESIZE - 2) + 1;
			}
			m_map[sP][0].isPath = true;
			m_startPosition = m_groundPosition[sP][0];
			m_startRow = sP;
			m_startColumn = 0;
			m_player.setPDirection(PlayerDirection::WEST);
			m_pX = sP;
			m_pY = 0;
			break;
		case(WallDirection::WEST):
			while (m_map[sP][MAZESIZE - 2].isPath == false)
			{
				sP = rand() % (MAZESIZE - 2) + 1;
			}
			m_map[sP][MAZESIZE - 1].isPath = true;
			m_startPosition = m_groundPosition[sP][MAZESIZE - 1];
			m_startRow = sP;
			m_startColumn = MAZESIZE - 1;
			m_player.setPDirection(PlayerDirection::EAST);
			m_pX = sP;
			m_pY = MAZESIZE - 1;
			break;
		default:
			break;
	}
	m_currentPosition = m_startPosition;
}


void MazeGenerator::setEnemy()
{
	int i = rand() % MAZESIZE;
	int j = rand() % MAZESIZE;

	while (m_map[i][j].isPath == false)
	{
		i = rand() % MAZESIZE;
		j = rand() % MAZESIZE;
	}

	if (m_map[i][j - 1].isPath == true)
	{
		m_enemy.changeDirection(EnemyDirection::NORTH);
	}

	else if (m_map[i][j + 1].isPath == true)
	{
		m_enemy.changeDirection(EnemyDirection::SOUTH);
	}

	else if (m_map[i - 1][j].isPath == true)
	{
		m_enemy.changeDirection(EnemyDirection::WEST);
	}

	else if (m_map[i + 1][j].isPath == true)
	{
		m_enemy.changeDirection(EnemyDirection::SOUTH);
	}

	m_enemy.setPosition(m_wCirclesCenter[i][j] - sf::Vector2f(50,50));
}

//Clears out unnecessary walls
void MazeGenerator::cleanMaze()
{
	for (int i = 0; i < MAZESIZE - 1; i++)
	{
		for (int j = 0; j < MAZESIZE - 1; j++)
		{
			if (i == 0)
			{
				m_map[i][j].leftWall = false;
			}

			if (i + 1 == MAZESIZE - 1)
			{
				m_map[i + 1][j].rightWall = false;
			}

			if (j == 0)
			{
				m_map[i][j].topWall = false;
			}

			if (j + 1 == MAZESIZE - 1)
			{
				m_map[i][j + 1].bottomWall = false;
			}

			if (m_map[i][j].isPath == false)
			{
				if (m_map[i + 1][j].isPath == false)
				{
					m_map[i][j].rightWall = false;
					m_map[i + 1][j].leftWall = false;
				}
				if (m_map[i][j + 1].isPath == false)
				{
					m_map[i][j].bottomWall = false;
					m_map[i][j + 1].topWall = false;
				}
			}
		}
	}
}


void MazeGenerator::createIslands()
{
	while (m_isleCreate != 0)
	{
		int xP = rand() % (MAZESIZE - 2) + 1;
		int yP = rand() % (MAZESIZE - 2) + 1;

		if (m_map[xP][yP].isPath == false && (m_map[xP][yP].leftWall == true && m_map[xP][yP].rightWall == true)
			&& (m_map[xP][yP].bottomWall == false && m_map[xP][yP].topWall == false))
		{

			m_map[xP][yP].isPath = true;

			m_map[xP][yP].leftWall = false;
			m_map[xP - 1][yP].rightWall = false;

			m_map[xP][yP].rightWall = false;
			m_map[xP + 1][yP].leftWall = false;

			m_map[xP][yP].topWall = true;
			m_map[xP][yP - 1].bottomWall = true;

			m_map[xP][yP].bottomWall = true;
			m_map[xP][yP + 1].topWall = true;
			m_isleCreate--;
		}

		else if (m_map[xP][yP].isPath == false && (m_map[xP][yP].bottomWall == true && m_map[xP][yP].topWall == true)
			&& (m_map[xP][yP].leftWall == false && m_map[xP][yP].rightWall == false))
		{
			std::cout << "L/R: " << m_map[xP][yP].leftWall << " " << m_map[xP][yP].rightWall << std::endl;

			m_map[xP][yP].isPath = true;

			m_map[xP][yP].leftWall = true;
			m_map[xP - 1][yP].rightWall = true;

			m_map[xP][yP].rightWall = true;
			m_map[xP + 1][yP].leftWall = true;

			m_map[xP][yP].topWall = false;
			m_map[xP][yP - 1].bottomWall = false;

			m_map[xP][yP].bottomWall = false;
			m_map[xP][yP + 1].topWall = false;
			m_isleCreate--;
		}
	}
}

void MazeGenerator::createBreakableWalls()
{
	while (m_breakableWalls != 0)
	{
		int xP = rand() % (MAZESIZE - 2) + 1;
		int yP = rand() % (MAZESIZE - 2) + 1;

		if (m_map[xP][yP].isPath == false && m_map[xP][yP].breakable == false &&
			((m_map[xP][yP].leftWall == true && m_map[xP][yP].rightWall == true && m_map[xP][yP].topWall == false && m_map[xP][yP].bottomWall == false)
				|| 
				(m_map[xP][yP].bottomWall == true && m_map[xP][yP].topWall == true && m_map[xP][yP].leftWall == false && m_map[xP][yP].rightWall == false)))
		{
			m_map[xP][yP].breakable = true;
			m_wallCircles[xP][yP].setFillColor(sf::Color::Green);
			m_breakableWalls--;
		}
	}
}

void MazeGenerator::breakWall()
{
	switch (m_player.getPDirection())
	{
	case(PlayerDirection::NORTH):
		if (m_map[m_pX][m_pY - 1].breakable == true)
		{
			m_map[m_pX][m_pY - 1].bottomWall = false;
			m_map[m_pX][m_pY - 1].topWall = false;
			m_map[m_pX][m_pY - 1].leftWall = true;
			m_map[m_pX][m_pY - 1].rightWall = true;
			m_map[m_pX][m_pY - 1].isPath = true;
		}
		break;

	case(PlayerDirection::SOUTH):
		if (m_map[m_pX][m_pY + 1].breakable == true)
		{
			m_map[m_pX][m_pY + 1].bottomWall = false;
			m_map[m_pX][m_pY + 1].topWall = false;
			m_map[m_pX][m_pY + 1].leftWall = true;
			m_map[m_pX][m_pY + 1].rightWall = true;
			m_map[m_pX][m_pY + 1].isPath = true;
		}
		break;

	case(PlayerDirection::WEST):
		if (m_map[m_pX - 1][m_pY].breakable == true)
		{
			m_map[m_pX - 1][m_pY].bottomWall = true;
			m_map[m_pX - 1][m_pY].topWall = true;
			m_map[m_pX - 1][m_pY].leftWall = false;
			m_map[m_pX - 1][m_pY].rightWall = false;
			m_map[m_pX - 1][m_pY].isPath = true;
		}
		break;

	case(PlayerDirection::EAST):
		if (m_map[m_pX + 1][m_pY].breakable == true)
		{
			m_map[m_pX + 1][m_pY].bottomWall = true;
			m_map[m_pX + 1][m_pY].topWall = true;
			m_map[m_pX + 1][m_pY].leftWall = false;
			m_map[m_pX + 1][m_pY].rightWall = false;
			m_map[m_pX + 1][m_pY].isPath = true;
		}
		break;

	case(PlayerDirection::NORTHEAST):
		if (m_map[m_pX][m_pY - 1].breakable == true)
		{
			m_map[m_pX][m_pY - 1].bottomWall = false;
			m_map[m_pX][m_pY - 1].topWall = false;
			m_map[m_pX][m_pY - 1].leftWall = true;
			m_map[m_pX][m_pY - 1].rightWall = true;
			m_map[m_pX][m_pY - 1].isPath = true;
		}
		if (m_map[m_pX + 1][m_pY].breakable == true)
		{
			m_map[m_pX + 1][m_pY].bottomWall = true;
			m_map[m_pX + 1][m_pY].topWall = true;
			m_map[m_pX + 1][m_pY].leftWall = false;
			m_map[m_pX + 1][m_pY].rightWall = false;
			m_map[m_pX + 1][m_pY].isPath = true;
		}
		break;

	case(PlayerDirection::SOUTHEAST):
		if (m_map[m_pX][m_pY + 1].breakable == true)
		{
			m_map[m_pX][m_pY + 1].bottomWall = false;
			m_map[m_pX][m_pY + 1].topWall = false;
			m_map[m_pX][m_pY + 1].leftWall = true;
			m_map[m_pX][m_pY + 1].rightWall = true;
			m_map[m_pX][m_pY + 1].isPath = true;
		}
		if (m_map[m_pX + 1][m_pY].breakable == true)
		{
			m_map[m_pX + 1][m_pY].bottomWall = true;
			m_map[m_pX + 1][m_pY].topWall = true;
			m_map[m_pX + 1][m_pY].leftWall = false;
			m_map[m_pX + 1][m_pY].rightWall = false;
			m_map[m_pX + 1][m_pY].isPath = true;
		}
		break;

	case(PlayerDirection::NORTHWEST):

		if (m_map[m_pX][m_pY - 1].breakable == true)
		{
			m_map[m_pX][m_pY - 1].bottomWall = false;
			m_map[m_pX][m_pY - 1].topWall = false;
			m_map[m_pX][m_pY - 1].leftWall = true;
			m_map[m_pX][m_pY - 1].rightWall = true;
			m_map[m_pX][m_pY - 1].isPath = true;
		}

		if (m_map[m_pX - 1][m_pY].breakable == true)
		{
			m_map[m_pX - 1][m_pY].bottomWall = true;
			m_map[m_pX - 1][m_pY].topWall = true;
			m_map[m_pX - 1][m_pY].leftWall = false;
			m_map[m_pX - 1][m_pY].rightWall = false;
			m_map[m_pX - 1][m_pY].isPath = true;
		}
		break;

	case(PlayerDirection::SOUTHWEST):
		if (m_map[m_pX + 1][m_pY].breakable == true)
		{
			m_map[m_pX + 1][m_pY].bottomWall = true;
			m_map[m_pX + 1][m_pY].topWall = true;
			m_map[m_pX + 1][m_pY].leftWall = false;
			m_map[m_pX + 1][m_pY].rightWall = false;
			m_map[m_pX + 1][m_pY].isPath = true;
		}

		if (m_map[m_pX - 1][m_pY].breakable == true)
		{
			m_map[m_pX - 1][m_pY].bottomWall = true;
			m_map[m_pX - 1][m_pY].topWall = true;
			m_map[m_pX - 1][m_pY].leftWall = false;
			m_map[m_pX - 1][m_pY].rightWall = false;
			m_map[m_pX - 1][m_pY].isPath = true;
		}

		break;

	default:
		break;
	}
}

void MazeGenerator::setPath(sf::Texture &t_texture)
{
	sf::IntRect m_groundRect(0, 0, 258, 512);
	for (int row = 0; row < MAZESIZE; row++)
	{
		for (int col = 0; col < MAZESIZE; col++)
		{
			m_path[row][col].setTexture(t_texture);
			m_path[row][col].setTextureRect(m_groundRect);
			m_path[row][col].setPosition(m_groundPosition[row][col]);
			m_path[row][col].setOrigin(127, 344);
		}
	}
}

sf::Vector2f MazeGenerator::getStartPosition()
{
	return m_startPosition;
}

void MazeGenerator::drawWalls(sf::RenderWindow & t_window, int row, int col)
{
	if (m_map[row][col].isPath == false && (m_map[row][col].topWall == true && m_map[row][col].leftWall == true))
	{
		m_northCornerSprite[row][col].setPosition(m_groundPosition[row][col]);
	    t_window.draw(m_northCornerSprite[row][col]);	
	}
	else
	{
		if (m_map[row][col].isPath == false && (m_map[row][col].leftWall == true && m_map[row][col].rightWall == false))
		{
			m_westWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_westWallSprites[row][col]);
		}
		else if (m_map[row][col].isPath == false && m_map[row][col].leftWall == true)
		{
			if (col > 0 && col < (MAZESIZE - 1))
			{
				m_westWallSprites[row][col].setPosition(m_groundPosition[row][col]);
				t_window.draw(m_westWallSprites[row][col]);
			}
		}

		if (m_map[row][col].isPath == false && (m_map[row][col].topWall == true && m_map[row][col].bottomWall == false))
		{
			m_northWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_northWallSprites[row][col]);
		}
		else if (m_map[row][col].isPath == false && m_map[row][col].topWall == true)
		{
			if (row > 0 && row < (MAZESIZE - 1))
			{
				m_northWallSprites[row][col].setPosition(m_groundPosition[row][col]);
				t_window.draw(m_northWallSprites[row][col]);
			}
		}
		if (m_player.getPaused())
		{
			m_player.renderPauseScreen(t_window);
		}
	}


	if (m_map[row][col].isPath == false && m_map[row][col].bottomWall == true && m_map[row][col].rightWall == true)
	{
		m_southCornerSprite[row][col].setPosition(m_groundPosition[row][col]);
		t_window.draw(m_southCornerSprite[row][col]);
	}

	else
	{
		if (m_map[row][col].isPath == false && (m_map[row][col].bottomWall == true && m_map[row][col].topWall == false))
		{
			m_southWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_southWallSprites[row][col]);
		}
		else if (m_map[row][col].isPath == false && m_map[row][col].bottomWall == true)
		{
			if (row > 0 && row < (MAZESIZE - 1))
			{
				m_southWallSprites[row][col].setPosition(m_groundPosition[row][col]);
				t_window.draw(m_southWallSprites[row][col]);
			}
		}

		if (m_map[row][col].isPath == false && (m_map[row][col].rightWall == true && m_map[row][col].leftWall == false))
		{
			m_eastWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_eastWallSprites[row][col]);
		}
		else if (m_map[row][col].isPath == false && m_map[row][col].rightWall == true)
		{
			if (col > 0 && col < (MAZESIZE - 1))
			{
				m_eastWallSprites[row][col].setPosition(m_groundPosition[row][col]);
				t_window.draw(m_eastWallSprites[row][col]);
			}
		}
		if (m_player.getPaused())
		{
			m_player.renderPauseScreen(t_window);
		}
	}

	if (m_map[row][col].breakable == true)
	{
		if (m_map[row][col].topWall == true)
		{
			m_brokenNorthWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_brokenNorthWallSprites[row][col]);
		}

		if (m_map[row][col].leftWall == true)
		{
			m_brokenWestWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_brokenWestWallSprites[row][col]);
		}

		if (m_map[row][col].bottomWall == true)
		{
			m_brokenSouthWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_brokenSouthWallSprites[row][col]);
		}

		if (m_map[row][col].rightWall == true)
		{
			m_brokenEastWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			t_window.draw(m_brokenEastWallSprites[row][col]);
		}
	}
}

void MazeGenerator::generateWalls(sf::Texture &t_northTexture, sf::Texture &t_southTexture, sf::Texture &t_eastTexture, sf::Texture &t_westTexture, sf::Texture &t_northBTexture, sf::Texture &t_southBTexture, sf::Texture &t_eastBTexture, sf::Texture &t_westBTexture, sf::Texture &t_northCTexture, sf::Texture &t_southCTexture)
{
	sf::IntRect m_WallRect(0, 0, 256, 512);
	// Create the Walls 
	for (int row = 0; row < MAZESIZE; row++)
	{
		for (int col = 0; col < MAZESIZE; col++)
		{
			m_northWallSprites[row][col].setTexture(t_northTexture);
			m_northWallSprites[row][col].setTextureRect(m_WallRect);
			m_northWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_northWallSprites[row][col].setOrigin(127, 344);

			m_southWallSprites[row][col].setTexture(t_southTexture);
			m_southWallSprites[row][col].setTextureRect(m_WallRect);
			m_southWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_southWallSprites[row][col].setOrigin(127, 344);

			m_eastWallSprites[row][col].setTexture(t_eastTexture);
			m_eastWallSprites[row][col].setTextureRect(m_WallRect);
			m_eastWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_eastWallSprites[row][col].setOrigin(127, 344);

			m_westWallSprites[row][col].setTexture(t_westTexture);
			m_westWallSprites[row][col].setTextureRect(m_WallRect);
			m_westWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_westWallSprites[row][col].setOrigin(127, 344);

			m_brokenNorthWallSprites[row][col].setTexture(t_northBTexture);
			m_brokenNorthWallSprites[row][col].setTextureRect(m_WallRect);
			m_brokenNorthWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_brokenNorthWallSprites[row][col].setOrigin(127, 344);

			m_brokenSouthWallSprites[row][col].setTexture(t_southBTexture);
			m_brokenSouthWallSprites[row][col].setTextureRect(m_WallRect);
			m_brokenSouthWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_brokenSouthWallSprites[row][col].setOrigin(127, 344);

			m_brokenEastWallSprites[row][col].setTexture(t_eastBTexture);
			m_brokenEastWallSprites[row][col].setTextureRect(m_WallRect);
			m_brokenEastWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_brokenEastWallSprites[row][col].setOrigin(127, 344);

			m_brokenWestWallSprites[row][col].setTexture(t_westBTexture);
			m_brokenWestWallSprites[row][col].setTextureRect(m_WallRect);
			m_brokenWestWallSprites[row][col].setPosition(m_groundPosition[row][col]);
			m_brokenWestWallSprites[row][col].setOrigin(127, 344);

			m_northCornerSprite[row][col].setTexture(t_northCTexture);
			m_northCornerSprite[row][col].setTextureRect(m_WallRect);
			m_northCornerSprite[row][col].setPosition(m_groundPosition[row][col]);
			m_northCornerSprite[row][col].setOrigin(127, 344);

			m_southCornerSprite[row][col].setTexture(t_southCTexture);
			m_southCornerSprite[row][col].setTextureRect(m_WallRect);
			m_southCornerSprite[row][col].setPosition(m_groundPosition[row][col]);
			m_southCornerSprite[row][col].setOrigin(127, 344);
		}
	}
}

void MazeGenerator::update()
{
	for (int i = 0; i < MAZESIZE; i++)
	{
		for (int j = 0; j < MAZESIZE; j++)
		{
			if (m_map[i][j].m_playerPath == true)
			{
				calculateIntersectPoints(i, j);
			}
		}
	}

	findPlayerTile();
	findEnemyTile();

	for (int i = m_pX - 1; i < m_pX + 2; i++)
	{
		for (int j = m_pY - 1; j < m_pY + 2; j++)
		{
			if(i >= 0 && i <= MAZESIZE - 1
				&&
				j >= 0 && j <= MAZESIZE - 1)
				if (m_map[i][j].isPath == false)
				{
					m_player.collisionCheck(m_wallCircles[i][j], m_wCirclesCenter[i][j]);
				}
		}
	}
}

void MazeGenerator::calculateIntersectPoints(int t_x, int t_y)
{
	int m_xInter = 0;
	int m_yInter = 0;

	for (int j = t_y - 1; j < t_y + 3; j++)
	{
		for (int i = t_x - 1; i < t_x + 2; i++)
		{
			/*if (m_map[i][j].isPath == false && m_map[i + 1][j].isPath == false && (m_yInter == 0 || m_yInter == 3))
			{
				m_circleIntersectPoint[m_xInter][m_yInter] = sf::Vector2f(m_wCirclesCenter[i][j].x + 64, m_wCirclesCenter[i][j].y - 37.5);
			}*/
			
			/*if (m_map[i][j].isPath == false && m_map[i][j + 1].isPath == false && (m_yInter > 0 && m_yInter < 3))
			{
				m_circleIntersectPoint[m_xInter][m_yInter + 1] = sf::Vector2f(m_wCirclesCenter[i][j].x + 64, m_wCirclesCenter[i][j].y - 37.5);
			}*/
			/*if (m_map[i][j].isPath == false && m_map[i][j - 1].isPath == false && (m_yInter > 0 && m_yInter < 3))
			{
				m_circleIntersectPoint[m_xInter][m_yInter] = sf::Vector2f(m_wCirclesCenter[i][j].x + 64, m_wCirclesCenter[i][j].y - 37.5);
			}*/
			//std::cout << m_xInter << m_yInter << std::endl;
			m_xInter++;
		}
		m_xInter = 0;
		m_yInter++;

	}
}

bool MazeGenerator::getWall()
{
	for (int row = 0; row < MAZESIZE; row++)
	{
		for (int col = 0; col < MAZESIZE; col++)
		{
			return m_maze[row][col];
		}
	}
}

void MazeGenerator::findPlayerTile()
{
	for (int i = 0; i < MAZESIZE - 1; i++)
	{
		for (int j = 0; j < MAZESIZE - 1; j++)
		{
			m_map[i][j].m_playerPath = false;
		}
	}

	float lowestDistance = 0;
	int m_previousX = m_pX;
	int m_previousY = m_pY;


	for (int i = 0; i < MAZESIZE - 1; i++)
	{
		for (int j = 0; j < MAZESIZE - 1; j++)
		{
			if (m_groundPosition[i][j].y < m_player.getPosition().y)
			{
				float distanceChecked = sqrt(pow(m_player.getPosition().x - m_groundPosition[i][j].x, 2) + pow(m_player.getPosition().y - m_groundPosition[i][j].y, 2));

				if (lowestDistance == 0)
				{
					lowestDistance = distanceChecked;

					m_pX = i;
					m_pY = j;

					m_map[i][j].m_playerPath = true;
				}
				else if (distanceChecked < lowestDistance)
				{
					m_map[m_pX][m_pY].m_playerPath = false;

					lowestDistance = distanceChecked;

					m_pX = i;
					m_pY = j;

					m_map[i][j].m_playerPath = true;
				}
			}
		}
	}

	//Use to help the enemy track the player.
	if (m_pX != m_previousX || m_pY != m_previousY)
	{
		m_map[m_pX][m_pY].m_playerTracks = m_map[m_previousX][m_previousY].m_playerTracks + 1;
		m_highestPTrack = m_map[m_pX][m_pY].m_playerTracks;
	}
}

void MazeGenerator::findEnemyTile()
{
	for (int i = 0; i < MAZESIZE - 1; i++)
	{
		for (int j = 0; j < MAZESIZE - 1; j++)
		{
			m_map[i][j].m_enemyPath = false;
		}
	}

	for (int i = 0; i < MAZESIZE - 1; i++)
	{
		for (int j = 0; j < MAZESIZE - 1; j++)
		{
				float distanceChecked = sqrt(pow(m_enemy.getPosition().x - m_wCirclesCenter[i][j].x, 2) + pow(m_enemy.getPosition().y - m_wCirclesCenter[i][j].y, 2));
				
				if (distanceChecked < m_wCircleRadius && distanceChecked > m_enemy.m_eCircleRadius && m_map[i][j].isPath == true 
					&& m_enemy.getPosition().x < m_wCirclesCenter[i][j].x - 40 && m_enemy.getPosition().y < m_wCirclesCenter[i][j].y - 40)
				{
					m_map[m_eX][m_eY].m_enemyPath = false;

					m_eX = i;
					m_eY = j;

					m_map[i][j].m_enemyPath = true;
				}
		}
	}

	int lowestDistance = 0;

	for (int i = 0; i < MAZESIZE - 1; i++)
	{
		for (int j = 0; j < MAZESIZE - 1; j++)
		{
			if (m_groundPosition[i][j].y < m_enemy.getPosition().y)
			{
				float distanceChecked = sqrt(pow(m_enemy.getPosition().x - m_groundPosition[i][j].x, 2) + pow(m_enemy.getPosition().y - m_groundPosition[i][j].y, 2));

				if (lowestDistance == 0)
				{
					lowestDistance = distanceChecked;

					m_eRX = i;
					m_eRY = j;
				}
				else if (distanceChecked < lowestDistance)
				{

					lowestDistance = distanceChecked;

					m_eRX = i;
					m_eRY = j;
				}
			}
		}
	}



	std::cout << m_eX << " " << m_eY << std::endl;
}

sf::Vector2i MazeGenerator::getPlayerTile()
{
	return sf::Vector2i(m_pX, m_pY);
}

sf::Vector2i MazeGenerator::getEnemyTile()
{
	return sf::Vector2i(m_eX, m_eY);
}


void MazeGenerator::drawMaze(sf::RenderWindow & t_window)
{
	for (int row = 0; row < MAZESIZE; row++)
	{
		for (int col = 0; col < MAZESIZE; col++)
		{
			switch(m_map[row][col].isPath)
			{
			case(true):
				t_window.draw(m_path[row][col]);
			}
		}
	}
	for (int j = 0; j < MAZESIZE; j++)
	{
		int y = j;
		for (int i = 0; i < j + 1; i++)
		{
			if (m_map[i][y].m_playerPath == true)
			{
				m_player.render(t_window);
			}
			if (i == m_eRX && j == m_eRY)
			{
				m_enemy.render(t_window);
			}
			drawWalls(t_window, i, y);
			y--;
		}
	}

	for (int i = 1; i < MAZESIZE; i++)
	{
		int x = i;
		for (int j = MAZESIZE - 1; j > i - 1; j--)
		{
			if (m_map[x][j].m_playerPath == true)
			{
				m_player.render(t_window);
			}
			if (i == m_eRX && j == m_eRY)
			{
				m_enemy.render(t_window);
			}
			drawWalls(t_window, x, j);
			x++;
		}
	}
	//m_enemy.render(t_window);
}
