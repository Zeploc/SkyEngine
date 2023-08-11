// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Layers/Layer.h"

struct ImGuiContext;
class CUIWidget;

class ENGINE_API CUILayer : public CLayer
{
public:
	CUILayer(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual ~CUILayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnEvent(CEvent& Event) override;
	void AddWidget(TPointer<CUIWidget> InWidget);
	ImGuiContext* GetGuiContext() const { return GuiContext; }

protected:
	virtual bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	virtual bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	virtual bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	virtual bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	virtual bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	virtual bool OnKeyTypedEvent(CKeyTypedEvent& Event);
	virtual bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);

	std::vector<TPointer<CUIWidget>> Widgets;
	ImGuiContext* GuiContext;
};
