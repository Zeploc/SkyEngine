// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <Scene/Scene.h>

#include "Math/Vector2.h"

class UIText;
class TransformationWidget;

class EditorScene : public Scene
{
public:
	EditorScene(const std::string& InSceneName);
	void AddSampleEntities();

	void UpdateSelectedEntity();

	void Update() override;
	void RenderScene() override;
	
	void OpenFile();

	void LoadLevel(std::ifstream& OpenedLevelFile);

	void SaveAsNew();
	void SaveCurrentLevel();

	bool OnEvent(CEvent& Event) override;
protected:
	bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event) override;
	bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event) override;
	bool OnMouseMovedEvent(CMouseMovedEvent& Event) override;
	bool OnMouseScrolledEvent(CMouseScrolledEvent& Event) override;
	bool OnKeyPressedEvent(CKeyPressedEvent& Event) override;
	bool OnKeyReleasedEvent(CKeyReleasedEvent& Event) override;
public:
	bool Wireframe = false;
	
	TPointer<UIText> LevelNameText;

	TPointer<TransformationWidget> LocationBox;
	bool bRotatingAroundPoint = false;
	bool bLookingAround = false;
	bool bPanning = false;
	
	float CurrentFocusDistance = 7.0f;
	SVector2i PreviousMousePosition;
};
