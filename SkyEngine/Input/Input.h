// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //

#include "Camera/CameraManager.h"
#include "Math/Vector.h"

// TODO: Rename file to WindowInput

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API CWindowInput
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

	enum KeyEventType
	{
		Pressed,
		Repeat,
		Released
	};
	enum ModiferType
	{
		Alt = 1,
		Shift = 1 << 1,
		Control = 1 << 2
	};

	enum InputMouse
	{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
	};

	friend class CEngineWindow;

	SVector2i MousePos;
	unsigned int KeyState[350]; //255
	unsigned int MouseState[3];

	unsigned int MouseSHIFT = INPUT_RELEASED;
	unsigned int MouseCTRL = INPUT_RELEASED;
	unsigned int MouseALT = INPUT_RELEASED;

	TVector3<float> Axis = {0, 0, 0};

	std::vector<class XBOXController*> Players;

protected:
	CWindowInput();
	
	void Init(TWeakPointer<CEngineWindow> Window);

	void MouseInput(int x, int y);

	void MouseButton(int button, KeyEventType EventType, int mods);

	void ProcessKeys(int key, int scancode, KeyEventType EventType, int mods);
	
	void processNormalKeysDown(unsigned char key, int x, int y);

	void processNormalKeysUp(unsigned char key, int x, int y);

	void processSpecialKeys(int key, int x, int y);

	//void Joystick(unsigned int buttonMask, int x, int y, int z);
	void Update();

	std::string InputStateString(unsigned int State);

	bool bKBHit = false;
	unsigned char cLastKey;

private:
	~CWindowInput();
};
