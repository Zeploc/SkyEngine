// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Layers/ViewportLayer.h"
#include "Scene/EditorScene.h"

class CUIWidget;

class CEditorViewportLayer : public CViewportLayer
{
public:
	CEditorViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow);
	void OnUpdate() override;
	void OnRender() override;

	
	void AddViewportWidget(TPointer<CUIWidget> InWidget);
	SVector2i GetViewportSize() override;
	SVector2i GetViewportPosition() override;
	void OnEvent(CEvent& Event) override;
	
	TPointer<Entity> GetSelectedEntity() const { return SelectedEntity; }
	void SelectEntity(TPointer<Entity> HitEntity);

protected:
	bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	bool OnKeyTypedEvent(CKeyTypedEvent& Event);
	bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);
	bool OnWindowResizeEvent(CWindowResizeEvent& Event);
	

	void UpdateSelectedEntity();
	
	std::vector<TPointer<CUIWidget>> Widgets;
	TPointer<EditorScene> EditorScene;

	TPointer<Entity> SelectedEntity;
	
	float CurrentFocusDistance = 7.0f;
	
	bool bRotatingAroundPoint = false;
	bool bLookingAround = false;
	bool bPanning = false;
	bool bWireframe = false;
	
	SVector2i PreviousMousePosition;
};
