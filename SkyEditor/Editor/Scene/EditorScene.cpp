// Copyright Skyward Studios, Inc. All Rights Reserved.

// Library Includes //
#include "SEPCH.h"
#include "EditorScene.h"

#include "Render/Lighting.h"

// Engine Includes //
#include <fstream>

#include "Editor/Windows/EditorWindow.h"
#include "Editor/Windows/EditorWindowManager.h"
#include <Render/Meshes/Basic/Cube.h>
#include <Render/Meshes/Advanced/GeometryObject.h>
#include <Render/Meshes/Model/Model.h>
#include <Render/Particle/ParticleSystem.h>
#include <Render/Meshes/Basic/Plane.h>
#include <Render/Meshes/Basic/Pyramid.h>
#include <Render/Meshes/Basic/Sphere.h>

#include <Windows.h>

#include "EditorApp.h"
#include "Core/Application.h"
#include "Editor/EditorViewportCanvas.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/Material.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Shaders/UndefinedShader.h"
#include "Render/Shaders/ShaderManager.h"

EditorScene::EditorScene(const std::string& InSceneName) : Scene(InSceneName)
{
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i WindowSize = ApplicationWindow->GetSize();
	

	// LevelNameText = std::make_shared<UIText>(glm::vec2(WindowSize.X - 540, WindowSize.Y - 15.0f), 0.0f, glm::vec4(0.3, 0.3, 0.3, 1.0f), "Level Name", "Resources/Fonts/Roboto-Regular.ttf", 20, EANCHOR::BOTTOM_RIGHT);
	// AddUIElement(LevelNameText);

	// TPointer<UIText> TipText(new UIText({WindowSize.X - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, EANCHOR::TOP_RIGHT));
	// AddUIElement(TipText);

	Lighting::SetLightPosition({5, 5, 5});
	Lighting::SetSunDirection({3, -1, 5});
	
	EditorWindow* NewWindow = new EditorWindow("Outliner", ApplicationWindow, glm::vec2(300, 400), glm::vec2(0, 0));
	NewWindow->SetBackColour(glm::vec3(0.2, 0.6, 0.8));
		
	EditorWindow* ContentWindow = new EditorWindow("Content", ApplicationWindow, glm::vec2(600, 150), glm::vec2(0, WindowSize.Y - 150));
	ContentWindow->SetBackColour(glm::vec3(0.4, 0.4, 0.4));

	//EditorWindow* ExternalWindow = new EditorWindow("External Test", nullptr, glm::vec2(500, 300), glm::vec2(100, 100));
	//ExternalWindow->SetBackColour(glm::vec3(0.6, 0.3, 0.4));

}

void EditorScene::OnLoadScene()
{
	Scene::OnLoadScene();
	AddSampleEntities();
}

void EditorScene::AddSampleEntities()
{
	TPointer<CTexture> BrickTexture = GetGraphicsAPI()->GetTexture("Resources/Images/StoneWall_2x2.jpg");
	// TPointer<CMaterial_PBR> BrickMaterial = std::make_shared<CMaterial_PBR>();
	TPointer<TMaterial<CUndefinedShader>> BrickMaterial = std::make_shared<TMaterial<CUndefinedShader>>("BrickMaterial", ShaderManager::GetUndefinedShader("BaseProgram"));
	BrickMaterial->Params.DiffuseTexture = BrickTexture;
	
	// Would be nice to be able to copy an existing material as a template
	TPointer<CMaterial_PBR> ColouredBrickMaterial = std::make_shared<CMaterial_PBR>("ColouredBrickMaterial");
	ColouredBrickMaterial->Params.DiffuseTexture = BrickTexture;
	ColouredBrickMaterial->Params.DiffuseColour = {0.5f, 0.3f, 0.3f, 1.0f};
	
	TPointer<CMaterial_PBR> ColouredBrickPlaneMaterial = std::make_shared<CMaterial_PBR>("ColouredBrickPlaneMaterial");
	ColouredBrickPlaneMaterial->Params.DiffuseTexture = BrickTexture;
	ColouredBrickPlaneMaterial->Params.DiffuseColour = {0.5f, 0.3f, 0.3f, 1.0f};
	ColouredBrickPlaneMaterial->bTwoSided = true;
	
	TPointer<CTexture> CliffTexture = GetGraphicsAPI()->GetTexture("Resources/Images/SmoothCliff_1024.jpg");
	TPointer<CMaterial_PBR> CliffMaterial = std::make_shared<CMaterial_PBR>("CliffMaterial");
	CliffMaterial->Params.DiffuseTexture = BrickTexture;
	CliffMaterial->Params.DiffuseColour = {0.1f, 0.8f, 0.3f, 1.0f};
	

	TPointer<CMaterial_PBR> PlaneMaterial = std::make_shared<CMaterial_PBR>("PlaneMaterial");
	PlaneMaterial->Params.DiffuseColour = {0.5f, 0.5f, 0.5f, 1.0f};
	PlaneMaterial->bTwoSided = true;

	// TODO: Switch all below to make shared
	TPointer<Entity> SphereRaycastTest(new Entity(STransform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}));
	TPointer<CSphere> SphereRaycastMesh = std::make_shared<CSphere>(SphereRaycastTest, 2.0f, 2.0f, 2.0f, CliffMaterial);
	SphereRaycastTest->AddComponent(SphereRaycastMesh);
	AddEntity(SphereRaycastTest);
			
	TPointer<Entity> FloorEntity(new Entity({{0, 0, 0}, {0, -90, 0}, {1, 1, 1}}));
	//glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	const TPointer<CPlane> FloorPlanMesh = std::make_shared<CPlane>(FloorEntity, 50.0f, 50.0f, PlaneMaterial);
	FloorEntity->AddComponent(FloorPlanMesh);
	AddEntity(FloorEntity);
	
	// TPointer<ParticleSystem> ParticleBoy(new ParticleSystem({{20, 8, 10}, {0, 0, 0}, {1, 1, 1}}));
	// ParticleBoy->SetPositionRange({-5, 5}, {0, 0}, {-5, 5});
	// ParticleBoy->SetDirectionRange({0, 0}, {-1, -1}, {0, 0});
	// ParticleBoy->SetFalloffTime({100.0f, 100.0f});
	// ParticleBoy->SetFalloffRange({30, 40});
	// ParticleBoy->SetSpeedRange({1, 10});
	// ParticleBoy->ParticleSize = 0.8f;
	// ParticleBoy->Init(1000, "Resources/Images/raindrop.png");
	// AddEntity(ParticleBoy, true);
	//
	// TPointer<ParticleSystem> ParticleBoy2(new ParticleSystem({{20, 1, 20}, {0, 0, 0}, {1, 1, 1}}));
	// ParticleBoy2->Init(1000, "Resources/Images/Box.png");
	// AddEntity(ParticleBoy2, true);
	//
	// TPointer<Entity> GeomEnt(new Entity({{10, 6, 10}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	// TPointer<GeometryObject> GeomShape = std::make_shared<GeometryObject>(GeomEnt, SVector4(0.0, 0.9f, 0.3f, 1.0f));
	// GeomEnt->AddMesh(GeomShape);
	// AddEntity(GeomEnt, true);
	
	// TODO: Fix as broken with updated assimp
	// Pointer<Entity> ModelEntity(new Entity(FTransform{{10.0f, 2.0f, 0.0f}, {-90, -90, 0}, {0.2f, 0.2f, 0.2f}}, EANCHOR::CENTER));
	// Pointer<Model> ModelEntityMesh(new Model({1.0f, 1.0f, 1.0f, 1.0f}, "Resources/Models/theDude.DAE"));
	// ModelEntity->AddMesh(ModelEntityMesh);
	// ModelEntityMesh->SetLit(true);
	// ModelEntityMesh->SetReflection(true);
	// AddEntity(ModelEntity, true);
	
	TPointer<Entity> CubeEnty(new Entity(STransform{{10.0f, 4.0f, 4.0f}, {0, 0, 0}, {1, 1, 1}}));
	TPointer<CCube> CubyMesh(new CCube(CubeEnty, 3.0f, 3.0f, 3.0f, ColouredBrickMaterial));
	CubeEnty->AddComponent(CubyMesh);
	AddEntity(CubeEnty);
	
	TPointer<Entity> PyramidEntity(new Entity(STransform{{10.0f, 4.0f, 8.0f}, {0, 0, 0}, {1, 1, 1}}));
	TPointer<CPyramid> PyramidMesh(new CPyramid(PyramidEntity, 3.0f, 3.0f, 3.0f, ColouredBrickMaterial));
	PyramidEntity->AddComponent(PyramidMesh);
	// PyramidMesh->SetLit(false, true);
	// TODO: Identify and fix pyramid lighting
	AddEntity(PyramidEntity);
	
	TPointer<Entity> SphereEntity(new Entity(STransform{{10.0f, 4.0f, 12.0f}, {0, 0, 0}, {1, 1, 1}}));
	TPointer<CSphere> SphereMesh(new CSphere(SphereEntity, 2.0f, 2.0f, 2.0f, ColouredBrickMaterial));
	SphereEntity->AddComponent(SphereMesh);
	AddEntity(SphereEntity);
	
	TPointer<Entity> PlaneEntity = std::make_shared<Entity>(STransform{{10.0f, 4.0f, 16.0f}, {0, -90, 0}, {1, 1, 1}});
	TPointer<CPlane> PlaneMesh = std::make_shared<CPlane>(PlaneEntity, 2.0f, 2.0f, ColouredBrickPlaneMaterial);
	PlaneEntity->AddComponent(PlaneMesh);
	AddEntity(PlaneEntity);
}

void EditorScene::Update()
{	
	Scene::Update();
	// TODO:
	// LevelNameText->sText = SceneName;
	TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	
	// TODO: Change later?
	EditorWindowManager::UpdateWindows();
}

void EditorScene::RenderScene()
{
	Scene::RenderScene();
}

void EditorScene::OpenFile()
{
	// TODO: Open dialogue (Currently makes file not openable?)
	// use IFileSaveDialog
	// https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
	
	// OPENFILENAME ofn;
	// ZeroMemory(&ofn, sizeof(ofn));
	// char szFile[100];
	// ofn.lStructSize = sizeof(ofn);
	// ofn.hwndOwner = NULL; //CameraManager::GetInstance()->MainWindow
	// ofn.lpstrFile = szFile;
	// ofn.lpstrFile[0] = '\0';
	// ofn.nMaxFile = sizeof(szFile);
	// ofn.lpstrFilter = "Level (*.slvl)\0*.slvl\0Text\0*.TXT\0";
	// ofn.nFilterIndex = 1;
	// ofn.lpstrFileTitle = NULL;
	// ofn.nMaxFileTitle = 0;
	// ofn.lpstrInitialDir = "Resources/Levels/";
	// ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// if (!GetOpenFileNameA(&ofn))
	// {
	// 	// MessageBox(NULL, ofn.lpstrFile, "File Name", MB_OK);
	// 	return;
	// }

	/*std::filesystem::path CurrentPath(szFile);
	std::filesystem::path RootResources("D:\\Projects\\OpenGL\\Graphics Combination\\Graphics Combination\\");
	std::filesystem::path relativePath = CurrentPath.lexically_relative(RootResources);
	std::string relative = relativePath.string();
	std::replace(relative.begin(), relative.end(), '\\', '/');*/

	std::string FileName = SceneName == "Example" ? "Resources/Levels/Demo Scene.slvl" : "Resources/Levels/Example.slvl";// = szFile;
	std::string line;
	std::ifstream OpenedFile(FileName);//szFile); //
	if (!OpenedFile.is_open())
	{
		MessageBoxA(NULL, FileName.c_str(), "Failed to open", MB_OK);
		return;
	}

	LoadLevel(OpenedFile);	
	
	OpenedFile.close();
}

void EditorScene::LoadLevel(std::ifstream& OpenedLevelFile)
{
	// std::string LevelName = OpenedLevelFile[0].substr(1, OpenedLevelFile[0].length() - 2);
	// SceneName = LevelName;

	std::vector< TPointer<Entity>> EntitiesCopy = Entities;
	for (TPointer<Entity> CurrentEnt : EntitiesCopy)
	{
		
		DestroyEntity(CurrentEnt);
	}

	std::string Empty;
	std::getline(OpenedLevelFile, Empty, '[');
	std::getline(OpenedLevelFile, SceneName, ']');
	std::getline(OpenedLevelFile, Empty, '\n');
	
	while(OpenedLevelFile.peek() != EOF )
	{
		TPointer<Entity> NewEntity(new Entity(STransform()));
		OpenedLevelFile >> NewEntity;
		std::getline(OpenedLevelFile, Empty, '\n');
		
		TPointer<CMaterial_PBR> TestMaterial = std::make_shared<CMaterial_PBR>("TestMaterial");
		TestMaterial->Params.DiffuseColour = {0.7f, 0.4f, 0.3f, 1.0f};
		
		TPointer<CCube> CubeMesh = std::make_shared<CCube>(NewEntity, 3.0f, 3.0f, 3.0f, TestMaterial);
		NewEntity->AddComponent(CubeMesh);
		AddEntity(NewEntity);
	}
	
	// for (std::string Line : OpenedLevelFile)
	// {
	// 	std::string IsEntity = Line.substr(0, 8);
	// 	if (IsEntity == "[Entity]")
	// 	{
	// 		// Pointer<Entity> NewEntity;
	// 		// IsEntity >> NewEntity;
	// 		
	// 		Pointer<Entity> CubeEnty(new Entity(Line));
	// 		//Pointer<Sphere> SphereMesh(Sphere(2.0f, 2.0f, 2.0f, { 0.1f, 0.8f, 0.3f, 1.0f }));
	// 		Pointer<Cube> CubyMesh(new Cube(3.0f, 3.0f, 3.0f, {0.7f, 0.4f, 0.3f, 1.0f}));
	//
	// 		CubeEnty->AddMesh(CubyMesh);
	// 		CubeEnty->SetInitialEntity(true);
	// 		
	// 		//Pointer<GeometryObject> GeomShape(new GeometryObject({0.0, 0.9f, 0.3f, 1.0f}));
	// 		//CubeEnty->AddMesh(GeomShape);
	// 		
	// 		//CubyMesh->SetLit(true);
	// 		AddEntity(CubeEnty, true);
	// 	}
	// }
}

void EditorScene::SaveAsNew()
{
	//IFileSaveDialog
	
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	char szFile[100];
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = LPWSTR(szFile);
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = LPCWSTR("Level (*.slvl)\0*.slvl\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = LPWSTR("Untitled.slvl");
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = LPCWSTR("Resources/Levels/");
	ofn.Flags = OFN_PATHMUSTEXIST;// | OFN_FILEMUSTEXIST;
	if (!GetSaveFileNameA(LPOPENFILENAMEA(&ofn)))
	{
		// MessageBox(NULL, ofn.lpstrFile, "File Name", MB_OK);
		return;
	}
	MessageBoxA(NULL, LPCSTR(ofn.lpstrFile), "File Name (WIP)", MB_OK);
}

void EditorScene::SaveCurrentLevel()
{
	 std::ofstream LevelFile;
	 std::string LevelPath = "Resources/Levels/" + SceneName + ".slvl";
	 LevelFile.open(LevelPath);
	 if (!LevelFile.is_open())
     {
     	MessageBoxA(NULL, LevelPath.c_str(), "Failed to edit", MB_OK);
     	return;
     }
	 LevelFile << "[" + SceneName + "]\n";
	 for (TPointer<Entity> Entity : Entities)
	 {	 	
	 	LevelFile << Entity << "\n";
	 }

	LevelFile.close();
}
