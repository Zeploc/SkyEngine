// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Canvas/ViewportCanvas.h"
#include "Scene/EditorScene.h"

class CUIWidget;

class CEditorViewportCanvas : public CViewportCanvas
{
public:
	CEditorViewportCanvas(TWeakPointer<CEngineWindow> InOwningWindow);
	void OnUpdate() override;
	void OnRender() override;
	
	void AddViewportWidget(THardPointer<CUIWidget> InWidget);
	SVector2i GetViewportSize() override;
	SVector2i GetViewportPosition() override;
	void StartGizmoViewDrag();

	THardPointer<Entity> GetSelectedEntity() const { return SelectedEntity; }
	void SelectEntity(THardPointer<Entity> HitEntity, bool bFocusCamera = false);
	bool DeleteSelected();
	void CreateEntity(const std::string& MeshAsset = {});

protected:
	bool OnMouseButtonPressed(int MouseButton, int Mods) override;
	bool OnMouseButtonReleased(int MouseButton, int Mods) override;
	bool OnMouseMoved(SVector2i MousePos) override;
	bool OnMouseScrolled(float XOffset, float YOffset) override;
	void FocusEntity();
	bool OnKeyPressed(int KeyCode, int Mods, int RepeatCount) override;
	bool OnKeyTyped(int KeyCode, int Mods) override;
	bool OnKeyReleased(int KeyCode, int Mods) override;
	bool OnWindowResize(unsigned int Width, unsigned int Height) override;
	
	void UpdateSelectedEntity();
	bool PreRender() override;
	void PostRender() override;

	bool UsingSpectatorControls() const { return bUseSpectatorControls; }	
	void SpectatorUpdate();

	std::vector<THardPointer<CUIWidget>> Widgets;

	THardPointer<Entity> SelectedEntity;
	int GizmoMode = 7;
	int GizmoTransformSpace = 0;
	float CameraSpeed = 12.0f;
	float MouseSensitivity = 0.15f;
	
	float CurrentFocusDistance = 7.0f;
	
	bool bUseSpectatorControls = false;
	bool bRotatingAroundPoint = false;
	bool bDollyingOnFocusPoint = false;
	bool bPanning = false;
	/* When pressing shift and moving an object, the camera will maintain the offset */
	bool bGizmoViewDrag = false;
	SVector InitialViewLockOffset;
	
	bool bWireframe = false;
	
	SVector2i PreviousMousePosition;
};
