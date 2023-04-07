// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// OpenGL Library Includes //
#include <glm/common.hpp>

// Library Includes //
#include <map>
#include <string>
#include <vector>

class Input
{
public:
	enum InputState
	{
		INPUT_FIRST_RELEASE,
		// First frame of Up state
		INPUT_RELEASED,
		// Default State (Up)
		INPUT_FIRST_PRESS,
		// First frame of Down state
		INPUT_HOLD,
		// Key is held Down
	};

	enum InputMouse
	{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
	};

	glm::vec2 MousePos;
	unsigned int KeyState[350]; //255
	unsigned int MouseState[3];

	unsigned int MouseSHIFT = INPUT_RELEASED;
	unsigned int MouseCTRL = INPUT_RELEASED;
	unsigned int MouseALT = INPUT_RELEASED;

	glm::vec3 Axis = {0, 0, 0};

	std::vector<class XBOXController*> Players;

	void Init(struct GLFWwindow* Window);

	void processNormalKeysDown(unsigned char key, int x, int y);

	void processNormalKeysUp(unsigned char key, int x, int y);

	void processSpecialKeys(int key, int x, int y);

	void MouseInput(int x, int y);

	void MouseButton(int button, int action, int mods);

	void processKeys(struct GLFWwindow* window, int key, int scancode, int action, int mods);

	//void Joystick(unsigned int buttonMask, int x, int y, int z);
	void Update();

	std::string InputStateString(unsigned int State);

	void SetCursorVisible(bool _bIsVisible);

	void ToggleCursorVisible();

	bool GetCursorVisible() { return bCursorVisible; };

	bool bKBHit = false;
	unsigned char cLastKey;

private:
	bool bCursorVisible = true;

public:
	// Singleton
	static Input* GetInstance();

	static void DestoryInstance();

	// Prototypes //
	static void LprocessNormalKeysDown(unsigned char key, int x, int y);

	static void LprocessNormalKeysUp(unsigned char key, int x, int y);

	static void LprocessSpecialKeys(int key, int x, int y);

	static void LMouseInput(GLFWwindow* window, double x, double y);

	static void LMouseButton(GLFWwindow* window, int button, int action, int mods);

	static void LprocessKeys(GLFWwindow* window, int key, int scancode, int action, int mods);

	Input();

private:
	static Input* m_pInput;
	static std::map<int, Input*> m_pInputs;

	~Input();

	Input(const Input&); // Don't Implement
	void operator=(const Input&); // Don't implement
};
