// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

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
	bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	bool OnKeyTypedEvent(CKeyTypedEvent& Event);
	bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);
	bool OnWindowResizeEvent(CWindowResizeEvent& Event);
	
	SVector SkyColour;
};
