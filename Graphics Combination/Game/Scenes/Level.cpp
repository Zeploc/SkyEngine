// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Level.h"

// Engine Includes //
#include "Engine/Editor/EditorWindowManager.h"
#include "Engine/Entity/Button3DEntity.h"
#include "Engine/Input/Input.h"
#include "Engine/Render/Cube.h"
#include "Engine/Render/GeometryObject.h"
#include "Engine/Render/ParticleSystem.h"
#include "Engine/Render/Plane.h"
#include "Engine/Render/Sphere.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Sound/SoundManager.h"
#include "Engine/UI/UIButton.h"

// Local Includes //
#include "Engine/Editor/TransformationWidget.h"
#include "Engine/Render/FrameBuffer.h"
#include "Game/Animation/ssAnimatedModel.h"
#include "Game/Players/Player.h"

// Library Includes //
#include <commdlg.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "Engine/Camera/Camera.h"
#include "Engine/System/Utils.h"
// Prototypes

Level::Level(std::string sSceneName) : Scene(sSceneName)
{
	// Pause Screen elements
	std::shared_ptr<UIButton> QuitBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 20, Camera::GetInstance()->SCR_HEIGHT - 10.0f), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	QuitBtn->AddText("Quit", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, {0, 0});
	QuitBtn->BindPress(new FDelegate<Level>(this, &Level::QuitCall));
	AddUIElement(QuitBtn);

	std::shared_ptr<UIButton> SaveBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 150, Camera::GetInstance()->SCR_HEIGHT - 10.0f), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	SaveBtn->AddText("Save", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, {0, 0});
	SaveBtn->BindPress(new FDelegate<Level>(this, &Level::SaveCurrentLevel));
	AddUIElement(SaveBtn);

	std::shared_ptr<UIButton> OpenBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 280, Camera::GetInstance()->SCR_HEIGHT - 10.0f), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	OpenBtn->AddText("Open", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, {0, 0});
	OpenBtn->BindPress(new FDelegate<Level>(this, &Level::OpenFile));
	AddUIElement(OpenBtn);

	LevelNameText = std::make_shared<UIText>(UIText({Camera::GetInstance()->SCR_WIDTH - 410, Camera::GetInstance()->SCR_HEIGHT - 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "Level Name", "Resources/Fonts/Roboto-Regular.ttf", 20, Utils::BOTTOM_RIGHT));
	AddUIElement(LevelNameText);

	std::shared_ptr<UIText> TipText(new UIText({Camera::GetInstance()->SCR_WIDTH - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, Utils::TOP_RIGHT));
	AddUIElement(TipText);

	/*InitInfo TerrainInfo;
	TerrainInfo.HeightmapFilename = "Resources/Images/TerrainMap2.raw";
	TerrainInfo.HeightScale = 1;
	TerrainInfo.HeightOffset = 0;
	TerrainInfo.NumCols = 256;
	TerrainInfo.NumRows = 256;
	TerrainInfo.CellSpacing = 1.0f;
	LevelTerrain = std::make_shared<Terrain>(Terrain({ { 0, 0, 0 } ,{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER, TerrainInfo));
	AddEntity(LevelTerrain, true);*/

	Lighting::m_v3LightPosition = {0, 7, 0};
	Lighting::m_v3SunDirection = {3, -1, 5};

	std::shared_ptr<Entity> GeomPlatform = std::make_shared<Entity>(Entity({{10, 5, 13}, {90, 0, 0}, {1, 1, 1}}, Utils::CENTER));
	std::shared_ptr<Plane> GoemPlatformMesh = std::make_shared<Plane>(Plane(1, 1, {0.4f, 0.5f, 0.7f, 1.0f}));
	GeomPlatform->AddMesh(GoemPlatformMesh);
	GoemPlatformMesh->SetLit(true, true);
	GoemPlatformMesh->LightProperties.fAmbientStrength = 1.0f;
	GoemPlatformMesh->bCullFace = false;
	AddEntity(GeomPlatform, true);
	std::shared_ptr<Entity> DynamicBoxEntity = std::make_shared<Entity>(Entity({{0, 0, 0}, {90, 0, 0}, {1, 1, 1}}, Utils::CENTER));
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
	AddEntity(NewPlayer, true);

	std::shared_ptr<Entity> GeomEnt = std::make_shared<Entity>(Entity({{10, 6, 10}, {0, 0, 0}, {1, 1, 1}}, Utils::CENTER));
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

	//Camera::GetInstance()->EnableSpectatorControls(true);
	//Input::GetInstance()->SetCursorVisible(false);

	Camera::GetInstance()->SetCameraPos({-10, 10, 0});
	Camera::GetInstance()->SetCameraForwardVector({0, -0.5, 1.0f});

	NewFramy = std::make_shared<FrameBuffer>();

	std::shared_ptr<Entity> SphereRaycastTest = std::make_shared<Entity>(Entity(Utils::Transform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, Utils::CENTER));
	std::shared_ptr<Sphere> SphereMesh = std::make_shared<Sphere>(Sphere(2.0f, 2.0f, 2.0f, {0.1f, 0.8f, 0.3f, 1.0f}, "Resources/Images/SmoothCliff_1024.jpg"));
	SphereRaycastTest->AddMesh(SphereMesh);
	SphereMesh->SetLit(true);
	SphereMesh->SetReflection(true);
	AddEntity(SphereRaycastTest, true);

	std::shared_ptr<Button3DEntity> FowardCubeButton = std::make_shared<Button3DEntity>(Button3DEntity(Utils::Transform{{4.0f, 7.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, Utils::CENTER, 6.0f, 2.0f, 1.0f, glm::vec4(0.7, 0.5, 0.7, 1.0f), glm::vec4(0.4, 0.5, 0.3, 1.0f), "Resources/Images/StoneWall_2x2.jpg"));
	FowardCubeButton->BindPress(new FDelegate<Level>(this, &Level::ButtonBoiFuncorness));
	AddEntity(FowardCubeButton, true);

	std::shared_ptr<Entity> CubeEnty = std::make_shared<Entity>(Entity(Utils::Transform{{25.0f, 4.0f, 4.0f}, {0, 0, 0}, {1, 1, 1}}, Utils::CENTER));
	std::shared_ptr<Cube> CubyMesh = std::make_shared<Cube>(Cube(3.0f, 3.0f, 3.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	CubeEnty->AddMesh(CubyMesh);
	CubyMesh->SetLit(true);
	CubyMesh->SetReflection(true);
	AddEntity(CubeEnty, true);

	LocationBox = std::make_shared<TransformationWidget>(TransformationWidget(Utils::Transform{{0.0f, 0.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, this));
	//std::shared_ptr<Cube> BoxMesh = std::make_shared<Cube>(Cube(0.4f, 0.4f, 0.4f, { 0.9, 0.1, 0.1, 1.0f }));
	//LocationBox->AddMesh(BoxMesh);
	AddEntity(LocationBox, true);
	LocationBox->SetVisible(false, true);
	LocationBox->bRayCast = false;
}

Level::~Level()
{
}

void Level::Update()
{
	Scene::Update();

	LevelNameText->sText = SceneName;

	if (Input::GetInstance()->KeyState[GLFW_KEY_ESCAPE] == Input::INPUT_FIRST_PRESS) // Escape
	{
		//Input::GetInstance()->ToggleCursorVisible();
		//Camera::GetInstance()->EnableSpectatorControls(!Input::GetInstance()->GetCursorVisible());

		Input::GetInstance()->SetCursorVisible(true);
		Camera::GetInstance()->EnableSpectatorControls(false);
	}
	if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_FIRST_PRESS)
	{
		Input::GetInstance()->SetCursorVisible(false);
		Camera::GetInstance()->EnableSpectatorControls(true);
	}
	else if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_FIRST_RELEASE)
	{
		Input::GetInstance()->SetCursorVisible(true);
		Camera::GetInstance()->EnableSpectatorControls(false);
	}
	if (Input::GetInstance()->KeyState[GLFW_KEY_G] == Input::INPUT_FIRST_PRESS) // Escape
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
	if (Input::GetInstance()->KeyState[GLFW_KEY_F] == Input::INPUT_FIRST_PRESS) // Escape
	{
		if (LocationBox && LocationBox->SelectedEntity)
		{
			Camera::GetInstance()->SetCameraPos(LocationBox->SelectedEntity->transform.Position + (-Camera::GetInstance()->GetCameraForwardVector() * 7.0f));
		}
	}
	if (Input::GetInstance()->MouseALT == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseALT == Input::INPUT_HOLD)
	{
		Camera::GetInstance()->SetCameraPos(LocationBox->SelectedEntity->transform.Position + (-Camera::GetInstance()->GetCameraForwardVector() * 7.0f));
	}
	if (Input::GetInstance()->MouseSHIFT == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseSHIFT == Input::INPUT_HOLD)
	{
		if (LocationBox && LocationBox->SelectedEntity)
		{
			Camera::GetInstance()->SetCameraPos(LocationBox->SelectedEntity->transform.Position + (-Camera::GetInstance()->GetCameraForwardVector() * 7.0f));
		}
	}

	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_HOLD)
	{
		glm::vec3 rayStart = Camera::GetInstance()->GetCameraPosition();
		glm::vec3 rayDirection = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);
		glm::vec3 HitPos;
		std::vector<std::shared_ptr<Entity>> HitEntities;
		std::vector<glm::vec3> HitPosition;
		for (auto& Ent : Entities)
		{
			if (!Ent->bRayCast || !Ent->EntityMesh || !Ent->IsVisible())
			{
				continue; // Don't check for raycast
			}
			if (Utils::CheckHit(rayStart, rayDirection, Ent, HitPos))
			{
				HitEntities.push_back(Ent);
				HitPosition.push_back(HitPos);
			}
		}
		if (HitEntities.size() > 0)
		{
			int ClosestHitID = 0;
			for (int i = 0; i < HitEntities.size(); i++)
			{
				// If new hit is closer
				if (length(rayStart - HitPosition[i]) < length(rayStart - HitPosition[ClosestHitID]))
				{
					ClosestHitID = i;
				}
			}
			std::shared_ptr<Entity> HitEntity = HitEntities[ClosestHitID]; // Hit ent
			HitPos = HitPosition[ClosestHitID];

			if (LocationBox && Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS)
			{
				LocationBox->transform.Position = HitEntity->transform.Position;
				LocationBox->SelectedEntity = HitEntity;
				if (!LocationBox->IsVisible())
				{
					LocationBox->SetVisible(true);
				}
			}
		}
	}
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

void Level::OpenFile()
{
	//OPENFILENAME ofn;
	//ZeroMemory(&ofn, sizeof(ofn));
	//char szFile[100];
	//ofn.lStructSize = sizeof(ofn);
	//ofn.hwndOwner = NULL;
	//ofn.lpstrFile = szFile;
	//ofn.lpstrFile[0] = '\0';
	//ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = "Level (*.slvl)\0*.slvl\0Text\0*.TXT\0";
	//ofn.nFilterIndex = 1;
	//ofn.lpstrFileTitle = NULL;
	//ofn.nMaxFileTitle = 0;
	//ofn.lpstrInitialDir = NULL;
	//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//if (!GetOpenFileNameA(&ofn))
	//{
	//	return;
	//	//MessageBox(NULL, ofn.lpstrFile, "File Name", MB_OK);
	//}

	/*std::filesystem::path CurrentPath(szFile);
	std::filesystem::path RootResources("D:\\Projects\\OpenGL\\Graphics Combination\\Graphics Combination\\");
	std::filesystem::path relativePath = CurrentPath.lexically_relative(RootResources);
	std::string relative = relativePath.string();
	std::replace(relative.begin(), relative.end(), '\\', '/');*/

	std::vector<std::string> Lines;
	std::string line;
	std::ifstream OpenedFile("Resources/Levels/Example.slvl"); //SceneName + ".slvl"); //relative);// 
	if (OpenedFile.is_open())
	{
		while (getline(OpenedFile, line))
		{
			Lines.push_back(line);
			std::cout << line << '\n';
		}
		OpenedFile.close();
		LoadLevel(Lines);
	}
}

void Level::LoadLevel(std::vector<std::string> Lines)
{
	std::string LevelName = Lines[0].substr(1, Lines[0].length() - 2);
	SceneName = LevelName;

	/*std::vector< std::shared_ptr<Entity>> EntitiesCopy = Entities;
	for (std::shared_ptr<Entity> CurrentEnt : EntitiesCopy)
	{
		DestroyEntity(CurrentEnt);
	}*/

	for (std::string Line : Lines)
	{
		std::string IsEntity = Line.substr(0, 8);
		if (IsEntity == "[Entity]")
		{
			std::shared_ptr<Entity> CubeEnty = std::make_shared<Entity>(Entity(Line));
			//std::shared_ptr<Sphere> SphereMesh = std::make_shared<Sphere>(Sphere(2.0f, 2.0f, 2.0f, { 0.1f, 0.8f, 0.3f, 1.0f }));
			std::shared_ptr<Cube> CubyMesh = std::make_shared<Cube>(Cube(3.0f, 3.0f, 3.0f, {0.7f, 0.4f, 0.3f, 1.0f}));
			CubeEnty->AddMesh(CubyMesh);
			//CubyMesh->SetLit(true);
			AddEntity(CubeEnty, true);
		}
	}
}

void Level::SaveCurrentLevel()
{
	std::ofstream myfile;
	myfile.open("Resources/Levels/" + SceneName + ".slvl");
	myfile << "[" + SceneName + "]\n";
	for (std::shared_ptr<Entity> Entity : Entities)
	{
		myfile << Entity->EntityToString() << "\n";
	}

	myfile.close();
}

void Level::QuitCall()
{
	glfwSetWindowShouldClose(EditorWindowManager::MainWindow, true);
}

void Level::ButtonBoiFuncorness()
{
	std::cout << "BUTTON THINGIE BOI\n";
}
