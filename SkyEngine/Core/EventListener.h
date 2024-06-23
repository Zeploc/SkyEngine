// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Events/Event.h"

class IEventListener
{
public:
	virtual void OnEvent(CEvent& Event) = 0;
};
