// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Event.h"

// Credit: The Cherno

class ENGINE_API CWindowResizeEvent : public CEvent
{
public:
	CWindowResizeEvent(unsigned int InWidth, unsigned int InHeight)
		: Width(InWidth), Height(InHeight) {}

	inline unsigned int GetWidth() const { return Width; }
	inline unsigned int GetHeight() const { return Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << Width << ", " << Height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	unsigned int Width, Height;
};