// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <Scene/Scene.h>

#include "Math/Vector2.h"

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

	void LoadLevel(std::ifstream& OpenedLevelFile);

	void SaveAsNew();
	void SaveCurrentLevel();
	
	bool Wireframe = false;
	
	TPointer<UIText> LevelNameText;

	TPointer<TransformationWidget> LocationBox;
	
	float CurrentFocusDistance = 7.0f;
	SVector2 PreviousMousePosition;
};
