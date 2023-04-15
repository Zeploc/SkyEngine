// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Level.h"

// Engine Includes //
#include "Engine/Entity/Button3DEntity.h"
#include "Engine/Render/Cube.h"
#include "Engine/Render/GeometryObject.h"
#include "Engine/Render/ParticleSystem.h"
#include "Engine/Render/Plane.h"
#include "Engine/Render/Sphere.h"
#include "Engine/Scene/SceneManager.h"

// Local Includes //
#include "Engine/Render/FrameBuffer.h"
#include "Game/Animation/ssAnimatedModel.h"
#include "Game/Players/Player.h"

// Library Includes //
#include <filesystem>
#include <fstream>
#include <iostream>

#include "Terrain.h"
#include "Engine/Camera/CameraManager.h"
#include "Engine/System/Utils.h"
// Prototypes

Level::Level(std::string sSceneName) : Scene(sSceneName)
{
	// InitInfo TerrainInfo;
	// TerrainInfo.HeightmapFilename = "Resources/Images/TerrainMap2.raw";
	// TerrainInfo.HeightScale = 1;
	// TerrainInfo.HeightOffset = 0;
	// TerrainInfo.NumCols = 256;
	// TerrainInfo.NumRows = 256;
	// TerrainInfo.CellSpacing = 1.0f;
	// LevelTerrain = std::make_shared<Terrain>(Terrain({ { 0, 0, 0 } ,{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER, TerrainInfo));
	// AddEntity(LevelTerrain, true);

	Lighting::m_v3LightPosition = {0, 7, 0};
	Lighting::m_v3SunDirection = {3, -1, 5};

	std::shared_ptr<Entity> GeomPlatform = std::make_shared<Entity>(Entity({{10, 5, 13}, {90, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Plane> GoemPlatformMesh = std::make_shared<Plane>(Plane(1, 1, {0.4f, 0.5f, 0.7f, 1.0f}));
	GeomPlatform->AddMesh(GoemPlatformMesh);
	GoemPlatformMesh->SetLit(true, true);
	GoemPlatformMesh->LightProperties.fAmbientStrength = 1.0f;
	GoemPlatformMesh->bCullFace = false;
	AddEntity(GeomPlatform, true);
	std::shared_ptr<Entity> DynamicBoxEntity = std::make_shared<Entity>(Entity({{0, 0, 0}, {90, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	//glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	std::shared_ptr<Plane> TestImage = std::make_shared<Plane>(Plane(50, 50, {0.9f, 0.3f, 0.1f, 1.0f}));
	DynamicBoxEntity->AddMesh(TestImage);
	TestImage->bCullFace = false;
	AddEntity(DynamicBoxEntity, true);

	//std::shared_ptr<Entity> AnimBoyo = std::shared_ptr<Entity>(new Entity({ { 0, 5, 0 } ,{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	//std::shared_ptr<AnimatedModel> NewAnimModel = std::shared_ptr<AnimatedModel>(new AnimatedModel("Resources/Models/theDude_idle_run.DAE", "Resources/Images/theDude.png"));
	//AnimBoyo->AddMesh(NewAnimModel);
	//AddEntity(AnimBoyo, true);

	//std::shared_ptr<ssAnimatedModel> PointlessextraOriginalAnimatedModel = std::shared_ptr<ssAnimatedModel>(new ssAnimatedModel("Resources/Models/theDude_idle_run.DAE", "Resources/Images/theDude.png"));
	//OriginalAnimatedModel = std::shared_ptr<ssAnimatedModel>(new ssAnimatedModel("Resources/Models/theDude_idle_run.DAE", "Resources/Images/theDude.png"));

	NewPlayer = std::make_shared<Player>(Player({ 3, 0, 3 }));
	NewPlayer->TerrainRef = LevelTerrain;
	// std::shared_ptr<GeometryObject> PlayerGeomShape = std::make_shared<GeometryObject>(GeometryObject({0.0, 0.9f, 0.3f, 1.0f}));
	// NewPlayer->AddMesh(PlayerGeomShape);
	AddEntity(NewPlayer, true);

	std::shared_ptr<Entity> GeomEnt = std::make_shared<Entity>(Entity({{10, 6, 10}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<GeometryObject> GeomShape = std::make_shared<GeometryObject>(GeometryObject({0.0, 0.9f, 0.3f, 1.0f}));
	GeomEnt->AddMesh(GeomShape);
	AddEntity(GeomEnt, true);

	/*std::shared_ptr<Entity> TessBoy = std::make_shared<Entity>(Entity({ { 14, 16, 14 } ,{ 0, 0, 0 },{ 4, 4, 4 } }, Utils::CENTER));
	std::shared_ptr<TessMesh> TessShapo = std::make_shared<TessMesh>(TessMesh(5, 5, { 0.0, 0.9f, 0.3f, 1.0f }));
	TessBoy->AddMesh(TessShapo);
	AddEntity(TessBoy, true);*/

	std::shared_ptr<ParticleSystem> ParticleBoy = std::make_shared<ParticleSystem>(ParticleSystem({{0, 20, 0}, {0, 0, 0}, {1, 1, 1}}));
	ParticleBoy->SetPositionRange({-110, 110}, {0, 0}, {-110, 110});
	ParticleBoy->SetDirectionRange({0, 0}, {-1, -1}, {0, 0});
	ParticleBoy->SetFalloffTime({100.0f, 100.0f});
	ParticleBoy->SetFalloffRange({30, 40});
	ParticleBoy->SetSpeedRange({10, 30});
	ParticleBoy->ParticleSize = 0.8f;
	ParticleBoy->Init(1000, "Resources/Images/raindrop.png");
	AddEntity(ParticleBoy, true);
	
	// ## GPU PARTICLE, BREAKS ON RENDER ###
	//std::shared_ptr<ParticleSystemGPU> GParticleBoy = std::make_shared<ParticleSystemGPU>(ParticleSystemGPU({ { 128, 30, 128 } ,{ 0, 0, 0 },{ 1, 1, 1 } }));
	//AddEntity(GParticleBoy, true);

	std::shared_ptr<ParticleSystem> ParticleBoy2 = std::make_shared<ParticleSystem>(ParticleSystem({{20, 1, 20}, {0, 0, 0}, {1, 1, 1}}));
	ParticleBoy2->Init(1000, "Resources/Images/Box.png");
	AddEntity(ParticleBoy2, true);

	//int RandValue = rand() % 256000;
	//float FullValue = (float)(RandValue) / 1000.0f;

	//CameraManager::GetInstance()->EnableSpectatorControls(true);
	//Input::GetInstance()->SetCursorVisible(false);

	CameraManager::GetInstance()->SetCameraPos({0, 10, -20});
	CameraManager::GetInstance()->SetCameraForwardVector({0, -0.5, 1.0f});

	NewFramy = std::make_shared<FrameBuffer>();

	std::shared_ptr<Entity> SphereRaycastTest = std::make_shared<Entity>(Entity(FTransform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Sphere> SphereMesh = std::make_shared<Sphere>(Sphere(2.0f, 2.0f, 2.0f, {0.1f, 0.8f, 0.3f, 1.0f}, "Resources/Images/SmoothCliff_1024.jpg"));
	SphereRaycastTest->AddMesh(SphereMesh);
	SphereMesh->SetLit(true);
	SphereMesh->SetReflection(true);
	AddEntity(SphereRaycastTest, true);

	CubeButton = std::make_shared<Button3DEntity>(Button3DEntity(FTransform{{4.0f, 7.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER, 6.0f, 2.0f, 1.0f, glm::vec4(0.7, 0.5, 0.7, 1.0f), glm::vec4(0.4, 0.5, 0.3, 1.0f), "Resources/Images/StoneWall_2x2.jpg"));
	CubeButton->BindPress(this, &Level::ButtonBoiFuncorness);
	AddEntity(CubeButton, true);

	std::shared_ptr<Entity> CubeEnty = std::make_shared<Entity>(Entity(FTransform{{25.0f, 4.0f, 4.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Cube> CubyMesh = std::make_shared<Cube>(Cube(3.0f, 3.0f, 3.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	CubeEnty->AddMesh(CubyMesh);
	CubyMesh->SetLit(true);
	CubyMesh->SetReflection(true);
	AddEntity(CubeEnty, true);
}

Level::~Level()
{
}

void Level::Update()
{
	Scene::Update();
}

void Level::OnLoadScene()
{
	Scene::OnLoadScene();
}

void Level::RenderScene()
{
	Scene::RenderScene();

	if (OriginalAnimatedModel)
	{
		OriginalAnimatedModel->render(LevelTerrain);
	}
	//NewFramy->Render();
}

void Level::ButtonBoiFuncorness()
{
	std::cout << "BUTTON THINGIE BOI" << std::endl;
}
