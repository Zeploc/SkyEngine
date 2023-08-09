// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "SEPCH.h"
#include "Core/Core.h"

// Credit: The Cherno

// Events in Hazel are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType
{
	None = 0,
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication    = 1, // Not used, remove after confirmed
	EventCategoryInput          = 1 << 1,
	EventCategoryKeyboard       = 1 << 2,
	EventCategoryMouse          = 1 << 3,
	EventCategoryMouseButton    = 1 << 4
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class ENGINE_API CEvent
{
	friend class EventDispatcher;
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(EventCategory category)
	{
		return GetCategoryFlags() & category;
	}
protected:
	bool bHandled = false;
};

class EventDispatcher
{
	template<typename T>
	using EventFn = std::function<bool(T&)>;
public:
	EventDispatcher(CEvent& event)
		: Event(event)
	{
	}

	template<typename T>
	bool Dispatch(EventFn<T> func)
	{
		if (Event.GetEventType() == T::GetStaticType())
		{
			Event.bHandled = func(*(T*)&Event);
			return true;
		}
		return false;
	}
private:
	CEvent& Event;
};

inline std::ostream& operator<<(std::ostream& os, const CEvent& e)
{
	return os << e.ToString();
}
