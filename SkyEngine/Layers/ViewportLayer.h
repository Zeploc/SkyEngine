// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Layer.h"

#include "Math/Vector.h"
#include "Math/Vector2.h"

class ENGINE_API CViewportLayer : public CLayer
{
public:
	CViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow);
	~CViewportLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnEvent(CEvent& Event) override;

	virtual SVector2i GetViewportSize();
	virtual SVector2i GetViewportPosition();
	
protected:
	SVector SkyColour;
};
