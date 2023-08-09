// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Events/Event.h"

#pragma once

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class CEngineWindow;

class ENGINE_API CLayer
{
public:
	CLayer(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InDebugName = "Layer");
	virtual ~CLayer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent(CEvent& Event) = 0;
	virtual void OnRender() {}
	// TODO:
	// virtual void OnEvent() {}
	inline const std::string& GetName() const { return DebugName; }

protected:
	TWeakPointer<CEngineWindow> OwningWindow;
	std::string DebugName;
	
};
