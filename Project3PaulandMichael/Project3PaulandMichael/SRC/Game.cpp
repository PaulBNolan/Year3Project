#include "Game.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

//event.key.code == sf::Keyboard::Escape)

Game::Game() :
	//m_window{ sf::VideoMode{ 1920, 1080, 32 }, "The Path to the Ultimate Card Effect.", sf::Style::Fullscreen},
	m_window{ sf::VideoMode{ 1920, 1080, 32 }, "The Path to the Ultimate Card Effect."},
	m_gameStates{ GameStates::Licence }
{
	if (!m_playerIdleTexture.loadFromFile("./Resources/Images/Player/PlayerSpriteSheetIdle.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_playerSouthTexture.loadFromFile("./Resources/Images/Player/PlayerSpriteSheetSouth.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_playerNorthTexture.loadFromFile("./Resources/Images/Player/PlayerSpriteSheetNorth.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_playerNeNwTexture.loadFromFile("./Resources/Images/Player/PlayerSpriteSheetNE-NW.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_playerSeSwTexture.loadFromFile("./Resources/Images/Player/PlayerSpriteSheetSE-SW.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_playerEWTexture.loadFromFile("./Resources/Images/Player/PlayerSpriteSheetE-W.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_enemyTexture.loadFromFile("./Resources/Images/Enemy/EnemySheetIdle.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_enemyTextureSouthEast.loadFromFile("./Resources/Images/Enemy/EnemySheetSouthEast.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_enemyTextureNorthWest.loadFromFile("./Resources/Images/Enemy/EnemySheetNorthWest.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_enemyRTextureSouthEast.loadFromFile("./Resources/Images/Enemy/EnemySheetSERunningSprites.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_enemyRTextureNorthWest.loadFromFile("./Resources/Images/Enemy/EnemySheetNWRunningSprites.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_bgTexture.loadFromFile("./Resources/Images/Menu/bricks.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_pathTexture.loadFromFile("./Resources/Images/Isometric/stone_N.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_northWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWall_N.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_southWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWall_S.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_eastWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWall_E.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_westWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWall_W.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_brokenNorthWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWallBroken_N.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_brokenSouthWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWallBroken_S.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_brokenEastWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWallBroken_E.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_brokenWestWallTexture.loadFromFile("./Resources/Images/Isometric/stoneWallBroken_W.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_northCornerTexture.loadFromFile("./Resources/Images/Isometric/stoneWallCorner_N.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_southCornerTexture.loadFromFile("./Resources/Images/Isometric/stoneWallCorner_S.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_ArialBlackfont.loadFromFile("./Resources/Fonts/arial.ttf"))
	{
		std::string s("Error loading font");
		throw std::exception(s.c_str());
	}

	m_backgroundSprite.setTexture(m_bgTexture);
	m_backgroundSprite.setPosition(0, 0);
	m_backgroundSprite.setScale(1.5, 1.5);
	m_licence = new Licence(*this, m_ArialBlackfont, m_window, m_keyhandler, m_controller);
	m_menu = new Menu(*this, m_ArialBlackfont, m_window, m_keyhandler, m_controller);
	m_player = new Player(*this, m_playerIdleTexture, m_playerSouthTexture,m_playerNorthTexture,m_playerNeNwTexture,m_playerSeSwTexture,m_playerEWTexture,m_window, sf::Vector2f(0.0, 0.0), m_keyhandler, m_controller, m_ArialBlackfont);
	m_enemy = new Enemy(*this, m_enemyTexture, m_enemyTextureSouthEast, m_enemyTextureNorthWest,m_enemyRTextureSouthEast,m_enemyRTextureNorthWest );
	m_options = new Option(*this, m_ArialBlackfont, m_window, m_keyhandler, m_controller);
	m_tutorial = new Tutorial(*this, *m_player);
	m_testMaze = new MazeGenerator(*this, *m_player, *m_enemy);
	m_testMaze->generateWalls(m_northWallTexture, m_southWallTexture, m_eastWallTexture, m_westWallTexture, m_brokenNorthWallTexture, m_brokenSouthWallTexture, m_brokenEastWallTexture, m_brokenWestWallTexture, m_northCornerTexture, m_southCornerTexture);
	m_testMaze->setPath(m_pathTexture);
	m_window.setVerticalSyncEnabled(true);
	m_player->setPosition(m_testMaze->getStartPosition());
}


Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

void Game::processGameEvents(sf::Event & event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		m_keyhandler.updateKey(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		m_keyhandler.updateKey(event.key.code, false);
		break;
	}
}


void Game::update(sf::Time t_deltaTime)
{
	m_controller.update();
	switch (m_gameStates)
	{
	case GameStates::Licence:
		m_licence->update(t_deltaTime.asSeconds());
		break;
	case GameStates::Splash:
		break;
	case GameStates::Menu:
		m_menu->update(t_deltaTime.asSeconds());
		break;
	case GameStates::Option:
		m_options->update(t_deltaTime.asSeconds());
		break;
	case GameStates::Tutorial:
		m_player->update(t_deltaTime.asSeconds(), *m_testMaze);
		break;
	case GameStates::Gameplay:
		m_player->update(t_deltaTime.asSeconds(), *m_testMaze);
		if (m_player->getPaused() == false)
		{
			m_enemy->update(t_deltaTime.asSeconds(),*m_testMaze);
			m_testMaze->update();
		}
		if (m_enemy->collisionCheck(m_player->m_playerCircle, m_player->m_playerCenter) == true)
		{
			m_window.close();
		}
		break;
	case GameStates::Quit:
		m_window.close(); // exit the app
		break;
	default:
		break;
	}
}

void Game::render()
{
	m_window.clear();
	sf::View baseView = m_window.getDefaultView();
	switch (m_gameStates)
	{
	case GameStates::Licence:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y);
		m_window.setView(baseView);
		m_licence->render(m_window);
		m_window.setView(m_window.getDefaultView());
		break;
	case GameStates::Splash:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y);
		m_window.setView(baseView);
		m_window.setView(m_window.getDefaultView());
		break;
	case GameStates::Menu:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y);
		m_window.setView(baseView);
		m_window.draw(m_backgroundSprite);
		m_menu->render(m_window);
		m_window.setView(m_window.getDefaultView());
		break;
	case GameStates::Option:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y);
		m_window.setView(baseView);
		m_window.draw(m_backgroundSprite);
		m_options->render(m_window);
		m_window.setView(m_window.getDefaultView());
		break;
	case GameStates::Tutorial:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y * 2);
		baseView.setCenter(m_player->getPosition());
		m_window.setView(baseView);
		m_testMaze->drawMaze(m_window);
		m_window.setView(m_window.getDefaultView());
		break;
	case GameStates::Gameplay:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y * 2);
		baseView.setCenter(m_player->getPosition());
		m_window.setView(baseView);
		m_testMaze->drawMaze(m_window);
		m_window.setView(m_window.getDefaultView());
		if (m_player->getPaused() == true)
		{
			m_player->renderPauseScreen(m_window);
		}
		break;
	default:
		baseView.setSize(baseView.getSize().x, baseView.getSize().y);
		m_window.setView(baseView);
		m_window.setView(m_window.getDefaultView());
		break;
	}
	m_window.display();
}