// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Layer.h"

#include "Math/Vector.h"

class CViewportLayer : public CLayer
{
public:
	CViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow);
	~CViewportLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnEvent(CEvent& Event) override;

protected:
	SVector SkyColour;
};
