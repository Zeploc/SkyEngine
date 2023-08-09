// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Event.h"

// Credit: The Cherno

class ENGINE_API CKeyEvent : public CEvent
{
public:
	inline int GetKeyCode() const { return KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	CKeyEvent(int InKeycode)
		: KeyCode(InKeycode) {}

	int KeyCode;
};

class ENGINE_API CKeyPressedEvent : public CKeyEvent
{
public:
	CKeyPressedEvent(int InKeycode, int InRepeatCount)
		: CKeyEvent(InKeycode), RepeatCount(InRepeatCount) {}

	inline int GetRepeatCount() const { return RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << KeyCode << " (" << RepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int RepeatCount;
};

class ENGINE_API CKeyReleasedEvent : public CKeyEvent
{
public:
	CKeyReleasedEvent(int InKeycode)
		: CKeyEvent(InKeycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};