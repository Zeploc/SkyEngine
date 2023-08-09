// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Event.h"

// Credit: The Cherno

class ENGINE_API CMouseMovedEvent : public CEvent
{
public:
	CMouseMovedEvent(float InX, float InY)
		: MouseX(InX), MouseY(InY) {}

	inline float GetX() const { return MouseX; }
	inline float GetY() const { return MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << MouseX << ", " << MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float MouseX, MouseY;
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

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	CMouseButtonEvent(int InButton)
		: Button(InButton) {}

	int Button; // TODO: Switch to enum
};

class ENGINE_API CMouseButtonPressedEvent : public CMouseButtonEvent
{
public:
	CMouseButtonPressedEvent(int InButton)
		: CMouseButtonEvent(InButton) {}

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
	CMouseButtonReleasedEvent(int InButton)
		: CMouseButtonEvent(InButton) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};