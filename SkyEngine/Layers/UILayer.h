// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Layers/Layer.h"

class CUILayer : public CLayer
{
public:
	CUILayer(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual ~CUILayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void DisplayInfoStats();
	void OnRender() override;
	void OnEvent(CEvent& Event) override;

protected:
	virtual bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	virtual bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	virtual bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	virtual bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	virtual bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	virtual bool OnKeyTypedEvent(CKeyTypedEvent& Event);
	virtual bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);
};
