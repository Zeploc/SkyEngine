// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Engine/Scene/Scene.h"

class UIText;
class TransformationWidget;

class EditorScene : public Scene
{
public:
	EditorScene(std::string sSceneName);
	void AddSampleEntities();

	void UpdateSelectedEntity();

	void Update() override;
	void RenderScene() override;
	
	void OpenFile();

	void LoadLevel(std::vector<std::string> Lines);

	void SaveCurrentLevel();
	
	bool Wireframe = false;
	
	std::shared_ptr<UIText> LevelNameText;

	std::shared_ptr<TransformationWidget> LocationBox;
	
	float CurrentFocusDistance = 7.0f;
	Vector2 PreviousMousePosition;
};
