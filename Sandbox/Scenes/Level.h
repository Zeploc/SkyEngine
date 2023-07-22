// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Engine Includes //
#include "Scene/Scene.h"

class Terrain;
class Player;
class ssAnimatedModel;
class Button3DEntity;
class FrameBuffer;

class Level : public Scene
{
public:
	Level(std::string sSceneName);

	~Level();

	void Update() override;

	void OnLoadScene() override;

	void RenderScene() override;

	void ButtonBoiFuncorness();

	std::shared_ptr<Terrain> LevelTerrain;
	std::shared_ptr<Player> NewPlayer;
	std::shared_ptr<ssAnimatedModel> OriginalAnimatedModel;
	std::shared_ptr<Button3DEntity> CubeButton;

	std::shared_ptr<FrameBuffer> NewFramy;
};
