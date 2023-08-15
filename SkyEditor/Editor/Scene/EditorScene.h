// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <Scene/Scene.h>

#include "Math/Vector2.h"

class UIText;
class CTransformationWidget;

class EditorScene : public Scene
{
public:
	EditorScene(const std::string& InSceneName);
	void AddSampleEntities();

	void Update() override;
	void RenderScene(TPointer<IGraphicsInstance> InGraphicsInstance) override;
	
	void OpenFile();

	void LoadLevel(std::ifstream& OpenedLevelFile);

	void SaveAsNew();
	void SaveCurrentLevel();

	void OnEvent(CEvent& Event) override;
protected:
	bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event) override;
	bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event) override;
	bool OnMouseMovedEvent(CMouseMovedEvent& Event) override;
	bool OnMouseScrolledEvent(CMouseScrolledEvent& Event) override;
	bool OnKeyPressedEvent(CKeyPressedEvent& Event) override;
	bool OnKeyReleasedEvent(CKeyReleasedEvent& Event) override;
public:
	
	TPointer<UIText> LevelNameText;

	TPointer<CTransformationWidget> TransformationWidget;
};
