// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Event.h"

// Credit: The Cherno

class ENGINE_API CKeyEvent : public CEvent
{
public:
	inline int GetKeyCode() const { return KeyCode; }
	inline int GetMods() const { return Mods; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	CKeyEvent(int InKeycode, int InMods)
		: KeyCode(InKeycode), Mods(InMods) {}

	int KeyCode;
	int Mods;
};

class ENGINE_API CKeyPressedEvent : public CKeyEvent
{
public:
	CKeyPressedEvent(int InKeycode, int InMods, int InRepeatCount)
		: CKeyEvent(InKeycode, InMods), RepeatCount(InRepeatCount) {}

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
	CKeyReleasedEvent(int InKeycode, int InMods)
		: CKeyEvent(InKeycode, InMods) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};