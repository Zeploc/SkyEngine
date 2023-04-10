// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Engine/Scene/Scene.h"

class UIText;
class TransformationWidget;

class EditorScene : public Scene
{
public:
	EditorScene(std::string sSceneName);
	
	void Update() override;
	void RenderScene() override;
	
	void OpenFile();

	void LoadLevel(std::vector<std::string> Lines);

	void SaveCurrentLevel();

	void QuitApplication();
	
	bool Wireframe = false;
	
	std::shared_ptr<UIText> LevelNameText;

	std::shared_ptr<TransformationWidget> LocationBox;
};
