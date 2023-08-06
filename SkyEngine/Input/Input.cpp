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
#include "Core/EngineWindow.h"
#include "Platform/Window/GLFW/GLFWWindow.h"

// Static Variables //
std::map<int, Input*> Input::m_pInputs;
Input* Input::m_pInput = nullptr;

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
Input* Input::GetInstance()
{
	//int currentWindow = glutGetWindow();
	//Input* CurrentFound = nullptr;
	//auto it = m_pInputs.find(currentWindow);
	//if (it == m_pInputs.end())// null or doesn't exist
	//{
	//	Input* NewInput = new Input;
	//	NewInput->Init();
	//	m_pInputs.insert(std::pair<int, Input*>(currentWindow, NewInput));
	//	CurrentFound = NewInput;
	//}
	//else
	//	CurrentFound = (*it).second;

	//return CurrentFound;

	if (!m_pInput)
	{
		m_pInput = new Input;
	}
	return m_pInput;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Input::DestoryInstance()
{
	//int currentWindow = glutGetWindow();
	//auto it = m_pInputs.find(currentWindow);
	//if (it._Ptr)// exists
	//{
	//	delete (*it).second;
	//	m_pInputs.erase(currentWindow);
	//}
	if (m_pInput)
	{
		delete m_pInput;
	}
	m_pInput = nullptr;
}

/************************************************************
#--Description--#: 	Initialises the OpenGL inputs to the local functions
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Input::Init(TPointer<EngineWindow> Window)
{
	// TODO: Temp until input interface is made generic
	TPointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(Window->GetGraphicsWindow());
	GLFWwindow* glfwWindow = GlfwWindow->GetGlWindow();

	glfwSetKeyCallback(glfwWindow, LprocessKeys);
	glfwSetCursorPosCallback(glfwWindow, LMouseInput);
	glfwSetMouseButtonCallback(glfwWindow, LMouseButton);
	/*
	glutSpecialFunc(Input::LprocessSpecialKeys);*/
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
	MousePos = glm::tvec2(x, y);
}

/************************************************************
#--Description--#: 	Proccess Mouse buttons
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the the button and the state and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::MouseButton(int button, int action, int mods)
{
	//MousePos = glm::vec2(x, y);
	if (button < 3)
	{
		if (action == GLFW_PRESS)
		{
			MouseState[button] = INPUT_FIRST_PRESS;
		}
		else if (action == GLFW_RELEASE)
		{
			MouseState[button] = INPUT_FIRST_RELEASE;
		}
		else
		{
			std::cout << "Unknown mouse state\n";
		}
	}

	if (mods & GLFW_MOD_SHIFT)
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
	if (mods & GLFW_MOD_ALT)
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
	if (mods & GLFW_MOD_CONTROL)
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

void Input::processKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bKBHit = true;
	cLastKey = key;
	if (action == GLFW_PRESS)
	{
		if (KeyState[key] == (INPUT_RELEASED | INPUT_FIRST_RELEASE))
		{
			KeyState[key] = INPUT_FIRST_PRESS;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		if (KeyState[key] == (INPUT_HOLD | INPUT_FIRST_PRESS))
		{
			KeyState[key] = INPUT_FIRST_RELEASE;
		}
	}

	if (mods & GLFW_MOD_SHIFT)
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
void Input::Update()
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
std::string Input::InputStateString(unsigned int State)
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

void Input::SetCursorVisible(bool _bIsVisible)
{
	// TODO: Temp until input interface is made generic	
	TPointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(GetApplication()->GetApplicationWindow()->GetGraphicsWindow());
	GLFWwindow* glfwWindow = GlfwWindow->GetGlWindow();
	
	if (_bIsVisible)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
	GetInstance()->processNormalKeysDown(key, x, y);
}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in key and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LprocessNormalKeysUp(unsigned char key, int x, int y)
{
	GetInstance()->processNormalKeysUp(key, x, y);
}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in key and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LprocessSpecialKeys(int key, int x, int y)
{
	GetInstance()->processSpecialKeys(key, x, y);
}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LMouseInput(GLFWwindow* window, double x, double y)
{
	GetInstance()->MouseInput((int)x, (int)y);
}

/************************************************************
#--Description--#:	Local function to pass opengl input to singleton input function
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in mouse key and state and mouse pos
#--Return--#: 		NA
************************************************************/
void Input::LMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	GetInstance()->MouseButton(button, action, mods);
}

void Input::LprocessKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetInstance()->processKeys(window, key, scancode, action, mods);
}
