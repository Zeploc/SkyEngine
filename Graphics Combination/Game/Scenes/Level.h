// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Engine Includes //
#include "Engine/Scene/Scene.h"

class Level : public Scene
{
public:
	Level(std::string sSceneName);

	~Level();

	void Update() override;

	void OnLoadScene() override;

	void RenderScene() override;

	void OpenFile();

	void LoadLevel(std::vector<std::string> Lines);

	void SaveCurrentLevel();

	void QuitCall();

	void ButtonBoiFuncorness();

	std::shared_ptr<class Terrain> LevelTerrain;
	std::shared_ptr<class Player> NewPlayer;
	std::shared_ptr<class ssAnimatedModel> OriginalAnimatedModel;
	std::shared_ptr<class UIText> LevelNameText;

	bool Wireframe = false;

	std::shared_ptr<class FrameBuffer> NewFramy;

	std::shared_ptr<class TransformationWidget> LocationBox;
};
