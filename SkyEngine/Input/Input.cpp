// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Input.h"

// OpenGL Library Includes //
#include <glew/glew.h>
#include <GLFW/glfw3.h>

// Engine Includes //
#include "CXBOXController.h"
#include "UI/Legacy/UIButton.h"

#include "Camera/CameraManager.h"
#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"

CInput::CInput()
{
	std::fill(KeyState, KeyState + 255, INPUT_RELEASED);
	std::fill(MouseState, MouseState + 3, INPUT_RELEASED);
}

CInput::~CInput()
{
}

void CInput::Init(TPointer<CEngineWindow> Window)
{
	/*
	glutSpecialFunc(CInput::LprocessSpecialKeys);*/
	//glutJoystickFunc(LJoystick, (float)GLUT_JOYSTICK_POLL_RATE / 100.0f);

	if (Players.size() <= 0)
	{
		Players.push_back(new XBOXController(1));
		Players.push_back(new XBOXController(2));
		Players.push_back(new XBOXController(3));
		Players.push_back(new XBOXController(4));
	}
}

/************************************************************
#--Description--#: 	Proccess keys down and sets KeyState values
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the key and the mouse pos
#--Return--#: 		NA
************************************************************/
void CInput::processNormalKeysDown(unsigned char key, int x, int y)
{
	bKBHit = true;
	cLastKey = key;
	if (KeyState[key] == (INPUT_RELEASED | INPUT_FIRST_RELEASE))
	{
		KeyState[key] = INPUT_FIRST_PRESS;
	}
}

/************************************************************
#--Description--#: 	Proccess keys up and sets KeyState values
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the key and the mouse pos
#--Return--#: 		NA
************************************************************/
void CInput::processNormalKeysUp(unsigned char key, int x, int y)
{
	if (KeyState[key] == (INPUT_HOLD | INPUT_FIRST_PRESS))
	{
		KeyState[key] = INPUT_FIRST_RELEASE;
	}
}

/************************************************************
#--Description--#: 	Proccess Special keys
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the key and the mouse pos
#--Return--#: 		NA
************************************************************/
void CInput::processSpecialKeys(int key, int x, int y)
{
}

/************************************************************
#--Description--#: 	Proccess Mouse movement and sets the mouse position vector
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the mouse pos
#--Return--#: 		NA
************************************************************/
void CInput::MouseInput(int x, int y)
{
	MousePos = glm::tvec2(x, y);
}

/************************************************************
#--Description--#: 	Proccess Mouse buttons
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the button and the state and mouse pos
#--Return--#: 		NA
************************************************************/
void CInput::MouseButton(int button, KeyEventType EventType, int mods)
{
	//MousePos = glm::vec2(x, y);
	if (button < 3)
	{
		if (EventType == Pressed)
		{
			MouseState[button] = INPUT_FIRST_PRESS;
		}
		else if (EventType == Released)
		{
			MouseState[button] = INPUT_FIRST_RELEASE;
		}
		else if (EventType == Repeat)
		{
			// TODO: mouse repeat
		}
		else
		{
			std::cout << "Unknown mouse state\n";
		}
	}

	if (mods & ModiferType::Shift)
	{
		if (MouseSHIFT != INPUT_HOLD && MouseSHIFT != INPUT_FIRST_PRESS)
		{
			MouseSHIFT = INPUT_FIRST_PRESS;
		}
		else
		{
			MouseSHIFT = INPUT_HOLD;
		}
	}
	else
	{
		if (MouseSHIFT == INPUT_HOLD)
		{
			MouseSHIFT = INPUT_FIRST_RELEASE;
		}
		else
		{
			MouseSHIFT = INPUT_RELEASED;
		}
	}
	if (mods & ModiferType::Alt)
	{
		if (MouseALT != INPUT_HOLD && MouseALT != INPUT_FIRST_PRESS)
		{
			MouseALT = INPUT_FIRST_PRESS;
		}
		else
		{
			MouseALT = INPUT_HOLD;
		}
	}
	else
	{
		if (MouseALT == INPUT_HOLD)
		{
			MouseALT = INPUT_FIRST_RELEASE;
		}
		else
		{
			MouseALT = INPUT_RELEASED;
		}
	}
	if (mods & ModiferType::Control)
	{
		if (MouseCTRL != INPUT_HOLD && MouseCTRL != INPUT_FIRST_PRESS)
		{
			MouseCTRL = INPUT_FIRST_PRESS;
		}
		else
		{
			MouseCTRL = INPUT_HOLD;
		}
	}
	else
	{
		if (MouseCTRL == INPUT_HOLD)
		{
			MouseCTRL = INPUT_FIRST_RELEASE;
		}
		else
		{
			MouseCTRL = INPUT_RELEASED;
		}
	}
}

void CInput::ProcessKeys(int key, int scancode, KeyEventType EventType, int mods)
{
	bKBHit = true;
	cLastKey = key;
	if (EventType == Pressed)
	{
		if (KeyState[key] == (INPUT_RELEASED | INPUT_FIRST_RELEASE))
		{
			KeyState[key] = INPUT_FIRST_PRESS;
		}
	}
	else if (EventType == Released)
	{
		if (KeyState[key] == (INPUT_HOLD | INPUT_FIRST_PRESS))
		{
			KeyState[key] = INPUT_FIRST_RELEASE;
		}
	}
	else if (EventType == Repeat)
	{
		// TODO: repeat
	}

	if (mods & ModiferType::Shift)
	{
		std::cout << "SHIFT DOWN\n";
	}
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void CInput::Update()
{
	for (int i = 0; i < 350; i++)
	{
		if (KeyState[i] == INPUT_FIRST_RELEASE)
		{
			KeyState[i] = INPUT_RELEASED;
		}
		if (KeyState[i] == INPUT_FIRST_PRESS)
		{
			KeyState[i] = INPUT_HOLD;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (MouseState[i] == INPUT_FIRST_RELEASE)
		{
			MouseState[i] = INPUT_RELEASED;
		}
		if (MouseState[i] == INPUT_FIRST_PRESS)
		{
			MouseState[i] = INPUT_HOLD;
		}
	}
	for (auto& player : Players)
	{
		player->Update();
	}

	bKBHit = false;
	UIButton::bButtonPressedThisFrame = false;
}

/************************************************************
#--Description--#: 	Converts enum value to string
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in enum value
#--Return--#: 		Returns enum name as string
************************************************************/
std::string CInput::InputStateString(unsigned int State)
{
	switch (State)
	{
		case INPUT_FIRST_RELEASE:
			return "INPUT_FIRST_RELEASE";
		case INPUT_RELEASED:
			return "INPUT_RELEASED";
		case INPUT_FIRST_PRESS:
			return "INPUT_FIRST_PRESS";
		case INPUT_HOLD:
			return "INPUT_HOLD";
		default:
			return "ERROR";
	}
}
