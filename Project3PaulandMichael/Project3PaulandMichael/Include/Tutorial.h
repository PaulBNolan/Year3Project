#pragma once
#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Game.h"

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

class Player;
class Game;

class Tutorial
{
public:
	Tutorial(Game & t_game, Player& t_player);
	~Tutorial();
	Game& m_game;
	Player& m_player;
};

