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
	void OnLoadScene() override;
	void AddSampleEntities();

	void Update() override;
	void RenderScene() override;
	
protected:
};
