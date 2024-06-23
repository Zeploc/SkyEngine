// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Event.h"
#include "Math/Vector2.h"

// Credit: The Cherno

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API CMouseMovedEvent : public CEvent
{
public:
	CMouseMovedEvent(SVector2i InPos)
		: MousePos(InPos) {}

	inline SVector2i GetMousePos() const { return MousePos; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << MousePos.ToString();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	SVector2i MousePos;
};

class ENGINE_API CMouseScrolledEvent : public CEvent
{
public:
	CMouseScrolledEvent(float InXOffset, float InYOffset)
		: XOffset(InXOffset), YOffset(InYOffset) {}

	inline float GetXOffset() const { return XOffset; }
	inline float GetYOffset() const { return YOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float XOffset, YOffset;
};

class ENGINE_API CMouseButtonEvent : public CEvent
{
public:
	inline int GetMouseButton() const { return Button; }
	inline int GetMods() const { return Mods; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	CMouseButtonEvent(int InButton, int InMods)
		: Button(InButton), Mods(InMods) {}

	int Button; // TODO: Switch to enum
	int Mods;
};

class ENGINE_API CMouseButtonPressedEvent : public CMouseButtonEvent
{
public:
	CMouseButtonPressedEvent(int InButton, int InMods)
		: CMouseButtonEvent(InButton, InMods) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class ENGINE_API CMouseButtonReleasedEvent : public CMouseButtonEvent
{
public:
	CMouseButtonReleasedEvent(int InButton, int InMods)
		: CMouseButtonEvent(InButton, InMods) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};