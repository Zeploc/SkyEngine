//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    Camera.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

#pragma once

// Engine Includes //
#include "Engine\Scene.h"

class Level : public Scene
{
public:
	Level(std::string sSceneName);
	~Level();

	virtual void Update() override;
	virtual void OnLoadScene() override;
	virtual void RenderScene() override;

	void OpenFile();

	void SaveCurrentLevel();

	void QuitCall();
	void ButtonBoiFuncorness();

	std::shared_ptr<class Terrain> LevelTerrain;
	std::shared_ptr<class Player> NewPlayer;
	std::shared_ptr<class ssAnimatedModel> OriginalAnimatedModel;

		
	bool Wireframe = false;

	std::shared_ptr<class FrameBuffer> NewFramy;

	std::shared_ptr<class TransformationWidget> LocationBox;
};

