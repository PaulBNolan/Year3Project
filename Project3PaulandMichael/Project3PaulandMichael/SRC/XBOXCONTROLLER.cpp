#include "XBOXCONTROLLER.h"
#include <iostream>

/// <summary>
/// @Authors: 
/// Michael Lyons, 
/// Paul Nolan
/// @version 1.0
/// </summary>

//Default Constructor
XboxController::XboxController()
{

}

XboxController::~XboxController()
{

}

/*will check for button presses within this method, get axis positions for both triggers,
get vector coords for both of the thumbsticks and will distinguish which dpad bools should
be true based on their axis coords*/
void XboxController::update()
{
	m_previousState = m_currentState;
	//face buttons
	m_currentState.A = sf::Joystick::isButtonPressed(0, 0);
	m_currentState.B = sf::Joystick::isButtonPressed(0, 1);
	m_currentState.X = sf::Joystick::isButtonPressed(0, 2);
	m_currentState.Y = sf::Joystick::isButtonPressed(0, 3);

	//bumpers, back, start and thumbstick clicks
	m_currentState.LB = sf::Joystick::isButtonPressed(0, 4);
	m_currentState.RB = sf::Joystick::isButtonPressed(0, 5);
	m_currentState.Back = sf::Joystick::isButtonPressed(0, 6);
	m_currentState.Start = sf::Joystick::isButtonPressed(0, 7);
	m_currentState.LeftThumbStickClick = sf::Joystick::isButtonPressed(0, 8);
	m_currentState.RightThumbStickClick = sf::Joystick::isButtonPressed(0, 9);

	//triggers and thumbsticks
	m_currentState.LTtrigger = (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z));
	m_currentState.RTtrigger = (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z));
	m_currentState.LeftThumbStick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y)));
	m_currentState.RightThumbStick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R), (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U)));

	if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) < -DPAD_THRESHOLD) == true)
	{
		m_currentState.DpadDown = true;    //if less than threshold then dpad is down
	}
	else {
		m_currentState.DpadDown = false;
	}
	if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) > DPAD_THRESHOLD) == true)
	{
		m_currentState.DpadUp = true; //if more than threshold then dpad is up
	}
	else {
		m_currentState.DpadUp = false;
	}

	if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) < -DPAD_THRESHOLD) == true)
	{
		m_currentState.DpadLeft = true;
	}
	else
	{
		m_currentState.DpadLeft = false;
	}
	if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) > DPAD_THRESHOLD) == true)
	{
		m_currentState.DpadRight = true;
	}
	else {
		m_currentState.DpadRight = false;
	}

}

/*simple method that will write out whether a controller is connected or not to the console*/
bool XboxController::isConnected()
{
	if (sf::Joystick::isConnected(0) == true)
	{
		std::cout << "Controller Connected!!!!";
		sf_Joystick_index = 0;
		return true;
	}
	else
	{
		std::cout << "No Controller Connected!!!!!!";
		return false;
	}

}