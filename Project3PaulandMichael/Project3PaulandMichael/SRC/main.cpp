/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// 
/// A game made using a proceedurally generated maze
/// Contains a enemy Ai
/// Lose condition is colliding with the enemy
/// Player can break walls
/// 
/// A game made using information we learned over the year from 
/// all of our subjects.
/// 
/// Total Time:
/// 240 Hours
/// Known Bugs: Slight issue with breaking walls
/// </summary>


#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include "Game.h"

int main()
{
	Game game;
	game.run();
}