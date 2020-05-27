//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    CSprite.cpp
// Description    	:    main implementation for CSprite
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <iostream>

// OpenGL Library Includes //
#include <glew.h>
#include <freeglut.h>

// Engine Includes //
#include "CXBOXController.h"
#include "Input.h"
#include "UIButton.h"

#include "EditorWindowManager.h"
#include "EditorWindow.h"

// Static Variables //
std::map<int, Input*> Input::m_pInputs;



/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Input::Input()
{
	std::fill(KeyState, KeyState + 255, INPUT_RELEASED);
	std::fill(MouseState, MouseState + 3, INPUT_RELEASED);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Input::~Input()
{
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
Input * Input::GetInstance()
{
	int currentWindow = glutGetWindow();
	Input* CurrentFound = nullptr;
	auto it = m_pInputs.find(currentWindow);
	if (it == m_pInputs.end())// null or doesn't exist
	{
		Input* NewInput = new Input;
		NewInput->Init();
		m_pInputs.insert(std::pair<int, Input*>(currentWindow, NewInput));
		CurrentFound = NewInput;
	}
	else
		CurrentFound = (*it).second;

	return CurrentFound;

	/*if (!m_pInput)
	{
		m_pInput = new Input;
	}
	return m_pInput;*/
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Input::DestoryInstance()
{
	int currentWindow = glutGetWindow();
	auto it = m_pInputs.find(currentWindow);
	if (it._Ptr)// exists
	{
		delete (*it).second;
		m_pInputs.erase(currentWindow);
	}
	/*if (m_pInput)
		delete m_pInput;
	m_pInput = nullptr;*/
}

/************************************************************
#--Description--#: 	Initialises the OpenGL inputs to the local functions
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Input::Init()
{
	glutKeyboardFunc(Input::LprocessNormalKeysDown);
	glutKeyboardUpFunc(Input::LprocessNormalKeysUp);
	glutSpecialFunc(Input::LprocessSpecialKeys);
	glutPassiveMotionFunc(Input::LMouseInput);
	glutMouseFunc(Input::LMouseButton);
	glutMotionFunc(Input::LMouseInput);
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
void Input::processNormalKeysDown(unsigned char key, int x, int y)
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
void Input::processNormalKeysUp(unsigned char key, int x, int y)
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
void Input::processSpecialKeys(int key, int x, int y)
{

}

/************************************************************
#--Description--#: 	Proccess Mouse movement and sets the mouse position vector
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the mouse pos
#--Return--#: 		NA
************************************************************/
void Input::MouseInput(int x, int y)
{
	MousePos = glm::vec2(x, y);
}

/************************************************************
#--Description--#: 	Proccess Mouse buttons
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the button and the state and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::MouseButton(int button, int state, int x, int y)
{
	MousePos = glm::vec2(x, y);
	if (button < 3)
	{
		if (state == GLUT_DOWN)
		{
			MouseState[button] = INPUT_FIRST_PRESS;
		}
		else if (state == GLUT_UP)
		{
			MouseState[button] = INPUT_FIRST_RELEASE;
		}
		else
		{
			std::cout << "Unknown mouse state\n";
		}
		
	}

}


/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Input::Update()
{
	for (int i = 0; i < 255; i++)
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
std::string Input::InputStateString(unsigned int State)
{
	switch (State)
	{
		case Input::INPUT_FIRST_RELEASE:
			return "INPUT_FIRST_RELEASE";
		case Input::INPUT_RELEASED:
			return "INPUT_RELEASED";
		case Input::INPUT_FIRST_PRESS:
			return "INPUT_FIRST_PRESS";
		case Input::INPUT_HOLD:
			return "INPUT_HOLD";
		default:
			return "ERROR";
	}
}

void Input::SetCursorVisible(bool _bIsVisible)
{
	if (_bIsVisible)
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	bCursorVisible = _bIsVisible;
}

void Input::ToggleCursorVisible()
{
	bCursorVisible = !bCursorVisible;
	SetCursorVisible(bCursorVisible);
}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function 
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in key and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LprocessNormalKeysDown(unsigned char key, int x, int y)
{
	Input::GetInstance()->processNormalKeysDown(key, x, y);

}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in key and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LprocessNormalKeysUp(unsigned char key, int x, int y)
{
	Input::GetInstance()->processNormalKeysUp(key, x, y);

}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in key and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LprocessSpecialKeys(int key, int x, int y)
{
	Input::GetInstance()->processSpecialKeys(key, x, y);

}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LMouseInput(int x, int y)
{
	Input::GetInstance()->MouseInput(x, y);

}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in mouse key and state and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LMouseButton(int button, int state, int x, int y)
{
	Input::GetInstance()->MouseButton(button, state, x, y);

}

