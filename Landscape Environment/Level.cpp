//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Menu.cpp
// Description    	:    main implementation for Menu
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//


// This Includes //
#include "Level.h"
#include "Player.h"

// Engine Includes //
#include "Engine\Plane.h"
#include "Engine\UIButton.h"
#include "Engine\SceneManager.h"
#include "Engine\Cube.h"
#include "Engine\Sphere.h"
#include "Engine\Input.h"
#include "Engine\LogManager.h"
#include "Engine\Time.h"
#include "Engine\SoundManager.h"
#include "Engine/Input.h"
#include "Engine\GeometryObject.h"
#include "Engine/ParticleSystem.h"
#include "Engine/ParticleSystemGPU.h"

// Local Includes //
#include "Player.h"
#include "Terrain.h"
#include "TessMesh.h"
#include "Engine/FrameBuffer.h"
#include "ssAnimatedModel.h"
#include "Engine/AnimatedModel.h"

// Library Includes //
#include <iostream>
#include <glm\gtx\string_cast.hpp>
// Prototypes

void QuitCall();

Level::Level(std::string sSceneName) : Scene(sSceneName)
{
	// Pause Screen elements
	std::shared_ptr<UIButton> QuitBtn(new UIButton(glm::vec2(20, Camera::GetInstance()->SCR_HEIGHT - 20.0f), Utils::BOTTOM_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 260, 50, QuitCall));
	QuitBtn->AddText("QUIT", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	AddUIElement(QuitBtn);

	std::shared_ptr<UIText> TipText(new UIText({ Camera::GetInstance()->SCR_WIDTH - 20, Camera::GetInstance()->SCR_HEIGHT - 30.0f }, 0, { 0.3, 0.3, 0.3, 1.0f }, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 26, Utils::BOTTOM_RIGHT));
	AddUIElement(TipText);

	InitInfo TerrainInfo;
	TerrainInfo.HeightmapFilename = "Resources/Images/TerrainMap2.raw";
	TerrainInfo.HeightScale = 1;
	TerrainInfo.HeightOffset = 0;
	TerrainInfo.NumCols = 256;
	TerrainInfo.NumRows = 256;
	TerrainInfo.CellSpacing = 1.0f;
	LevelTerrain = std::make_shared<Terrain>(Terrain({ { 0, 0, 0 } ,{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER, TerrainInfo));
	AddEntity(LevelTerrain, true);

	Lighting::m_v3LightPosition = { 10, 10, 10 };
	Lighting::m_v3SunDirection = { 3, -1, 5 };

	std::shared_ptr<Entity> DynamicBoxEntity = std::make_shared<Entity>(Entity({ { 0, -10, 0 } ,{ 90, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	std::shared_ptr<Plane> TestImage = std::make_shared<Plane>(Plane(Points, { 0.9f, 0.3f, 0.1f, 1.0f }));
	DynamicBoxEntity->AddMesh(TestImage);
	TestImage->bCullFace = false;
	AddEntity(DynamicBoxEntity, true);

	std::shared_ptr<Entity> GeomPlatform = std::make_shared<Entity>(Entity({ { 10, 11, 13 } ,{ 90, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	std::shared_ptr<Plane> GoemPlatformMesh = std::make_shared<Plane>(Plane(1, 1, { 0.4f, 0.5f, 0.7f, 1.0f }));
	GeomPlatform->AddMesh(GoemPlatformMesh);
	GoemPlatformMesh->SetLit(true, true);
	GoemPlatformMesh->LightProperties.fAmbientStrength = 1.0f;
	GoemPlatformMesh->bCullFace = false;
	AddEntity(GeomPlatform, true);

	//std::shared_ptr<Entity> AnimBoyo = std::shared_ptr<Entity>(new Entity({ { 0, 5, 0 } ,{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	//std::shared_ptr<AnimatedModel> NewAnimModel = std::shared_ptr<AnimatedModel>(new AnimatedModel("Resources/Models/theDude_idle_run.DAE", "Resources/Images/theDude.png"));
	//AnimBoyo->AddMesh(NewAnimModel);
	//AddEntity(AnimBoyo, true);

	//std::shared_ptr<ssAnimatedModel> PointlessextraOriginalAnimatedModel = std::shared_ptr<ssAnimatedModel>(new ssAnimatedModel("Resources/Models/theDude_idle_run.DAE", "Resources/Images/theDude.png"));
	//OriginalAnimatedModel = std::shared_ptr<ssAnimatedModel>(new ssAnimatedModel("Resources/Models/theDude_idle_run.DAE", "Resources/Images/theDude.png"));


	NewPlayer = std::make_shared<Player>(Player({ 3, 0, 3 }));
	NewPlayer->TerrainRef = LevelTerrain;
	AddEntity(NewPlayer, true);


	std::shared_ptr<Entity> GeomEnt = std::make_shared<Entity>(Entity({ { 10, 13, 10 } ,{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	std::shared_ptr<GeometryObject> GeomShape = std::make_shared<GeometryObject>(GeometryObject({0.0, 0.9f, 0.3f, 1.0f}));
	GeomEnt->AddMesh(GeomShape);
	AddEntity(GeomEnt, true);

	/*std::shared_ptr<Entity> TessBoy = std::make_shared<Entity>(Entity({ { 14, 16, 14 } ,{ 0, 0, 0 },{ 4, 4, 4 } }, Utils::CENTER));
	std::shared_ptr<TessMesh> TessShapo = std::make_shared<TessMesh>(TessMesh(5, 5, { 0.0, 0.9f, 0.3f, 1.0f }));
	TessBoy->AddMesh(TessShapo);
	AddEntity(TessBoy, true);*/
	
	std::shared_ptr<ParticleSystem> ParticleBoy = std::make_shared<ParticleSystem>(ParticleSystem({ {  128, 30, 128  } ,{ 0, 0, 0 },{ 1, 1, 1 } }));
	ParticleBoy->SetPositionRange({ -128, 128 }, { 0, 0 }, { -128, 128 });
	ParticleBoy->SetDirectionRange({ 0, 0 }, { -1, -1 }, { 0, 0 });
	ParticleBoy->SetFalloffTime({ 100.0f, 100.0f });
	ParticleBoy->SetFalloffRange({ 20, 30 });
	ParticleBoy->SetSpeedRange({ 10, 30 });
	ParticleBoy->ParticleSize = 0.8f;
	ParticleBoy->Init(1000, "Resources/Images/raindrop.png");
	AddEntity(ParticleBoy, true);

	// ## GPU PARTICLE, BREAKS ON RENDER ###
	//std::shared_ptr<ParticleSystemGPU> GParticleBoy = std::make_shared<ParticleSystemGPU>(ParticleSystemGPU({ { 128, 30, 128 } ,{ 0, 0, 0 },{ 1, 1, 1 } }));
	//AddEntity(GParticleBoy, true);


	std::shared_ptr<ParticleSystem> ParticleBoy2 = std::make_shared<ParticleSystem>(ParticleSystem({ { 20, 10, 20 } ,{ 0, 0, 0 },{ 1, 1, 1 } }));
	ParticleBoy2->Init(1000, "Resources/Images/Box.png");
	AddEntity(ParticleBoy2, true);

	//int RandValue = rand() % 256000;
	//float FullValue = (float)(RandValue) / 1000.0f;

	Camera::GetInstance()->EnableSpectatorControls(true);
	Camera::GetInstance()->bSpectatorMovement = false;
	Input::GetInstance()->SetCursorVisible(false);

	Camera::GetInstance()->SetCameraPos({ -10, 10, 0 });
	Camera::GetInstance()->SetCameraForwardVector({ 0, -0.5, 1.0f });

	NewFramy = std::make_shared<FrameBuffer>();

}

Level::~Level()
{

}

void Level::Update()
{

	Scene::Update();

	if (Input::GetInstance()->KeyState[27] == Input::INPUT_FIRST_PRESS) // Escape
	{
		Input::GetInstance()->ToggleCursorVisible();
		Camera::GetInstance()->EnableSpectatorControls(!Input::GetInstance()->GetCursorVisible());
	}
	if (Input::GetInstance()->KeyState[(unsigned char) 'g'] == Input::INPUT_FIRST_PRESS) // Escape
	{
		Wireframe = !Wireframe;
		if (Wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	

	Camera::GetInstance()->SetCameraPos(NewPlayer->transform.Position + (-Camera::GetInstance()->GetCameraForwardVector() * 7.0f) +  glm::vec3(0, 2, 0));

}

void Level::OnLoadScene()
{
	Scene::OnLoadScene();
	
}

void Level::RenderScene()
{	
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindFramebuffer(GL_FRAMEBUFFER, NewFramy->framebuffer);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::RenderScene();

	if (OriginalAnimatedModel) OriginalAnimatedModel->render(LevelTerrain);
	//NewFramy->Render();


}

void QuitCall()
{
	glutLeaveMainLoop();
}
