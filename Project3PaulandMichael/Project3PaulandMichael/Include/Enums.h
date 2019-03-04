/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

#ifndef MY_ENUMS
#define MY_ENUMS

/// <summary>
/// for switch between screens
/// </summary>
enum class GameStates
{
	Licence,
	Splash,
	Menu,
	Option,
	Tutorial,
	Gameplay,
	Quit
};

enum class WallDirection 
{ 
	NORTH, 
	SOUTH, 
	EAST,
	WEST 
};

enum class PlayerDirection
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	NORTHWEST,
	NORTHEAST,
	SOUTHWEST,
	SOUTHEAST
};

enum class EnemyDirection
{
	IDLE,
	NORTH,
	SOUTH,
	EAST,
	WEST
};
#endif // !MY_ENUMS
