// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#pragma once

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API CLayer
{
public:
	CLayer(const std::string& InDebugName = "Layer");
	virtual ~CLayer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnRender() {}
	// TODO:
	// virtual void OnEvent() {}
	inline const std::string& GetName() const { return DebugName; }

protected:
	std::string DebugName;
	
};
