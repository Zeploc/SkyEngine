// Copyright Skyward Studios, Inc. All Rights Reserved.

// Library Includes //
#include "SEPCH.h"
#include "EditorScene.h"

#include "Render/Lighting.h"

// Engine Includes //
#include <fstream>

#include <Camera/CameraManager.h>
#include <Input/Input.h>
#include <UI/Legacy/UIText.h>
#include <UI/Legacy/UIButton.h>

#include "TransformationWidget.h"
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

#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/Material.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Shaders/UndefinedShader.h"
#include "Render/Shaders/ShaderManager.h"

EditorScene::EditorScene(const std::string& InSceneName) : Scene(InSceneName)
{
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i WindowSize = ApplicationWindow->GetSize();

	TPointer<UIButton> QuitBtn(new UIButton(glm::vec2(WindowSize.X - 20, WindowSize.Y - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	QuitBtn->AddText("Quit", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	// TODO: Let delegate allow derived class type when passing in "this"? - Auto cast check inside? Template type for derived types allowed?
	QuitBtn->BindPress(GetApplication(), &SkyEngine::Application::Quit);
	AddUIElement(QuitBtn);
	
	TPointer<UIButton> SaveBtn(new UIButton(glm::vec2(WindowSize.X - 150, WindowSize.Y - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	SaveBtn->AddText("Save", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	SaveBtn->BindPress(this, &EditorScene::SaveCurrentLevel);
	AddUIElement(SaveBtn);
	
	TPointer<UIButton> SaveAsNewBtn(new UIButton(glm::vec2(WindowSize.X - 280, WindowSize.Y - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	SaveAsNewBtn->AddText("Save As", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	SaveAsNewBtn->BindPress(this, &EditorScene::SaveAsNew);
	AddUIElement(SaveAsNewBtn);

	TPointer<UIButton> OpenBtn(new UIButton(glm::vec2(WindowSize.X - 410, WindowSize.Y - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	OpenBtn->AddText("Open", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	OpenBtn->BindPress(this, &EditorScene::OpenFile);
	AddUIElement(OpenBtn);

	// LevelNameText = std::make_shared<UIText>(glm::vec2(WindowSize.X - 540, WindowSize.Y - 15.0f), 0.0f, glm::vec4(0.3, 0.3, 0.3, 1.0f), "Level Name", "Resources/Fonts/Roboto-Regular.ttf", 20, EANCHOR::BOTTOM_RIGHT);
	// AddUIElement(LevelNameText);

	TPointer<UIText> TipText(new UIText({WindowSize.X - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, EANCHOR::TOP_RIGHT));
	AddUIElement(TipText);

	Lighting::SetLightPosition({5, 5, 5});
	Lighting::SetSunDirection({3, -1, 5});
	
	CameraManager::GetInstance()->SetCameraPos({-10, 10, 0});
	CameraManager::GetInstance()->SetCameraForwardVector({0, -0.5, 1.0f});

	EditorWindow* NewWindow = new EditorWindow("Outliner", ApplicationWindow, glm::vec2(300, 400), glm::vec2(0, 0));
	NewWindow->SetBackColour(glm::vec3(0.2, 0.6, 0.8));
		
	EditorWindow* ContentWindow = new EditorWindow("Content", ApplicationWindow, glm::vec2(600, 150), glm::vec2(0, WindowSize.Y - 150));
	ContentWindow->SetBackColour(glm::vec3(0.4, 0.4, 0.4));

	//EditorWindow* ExternalWindow = new EditorWindow("External Test", nullptr, glm::vec2(500, 300), glm::vec2(100, 100));
	//ExternalWindow->SetBackColour(glm::vec3(0.6, 0.3, 0.4));

	AddSampleEntities();


	// Last added to appear on top
	// TODO: Depth test order to always be infront of loaded level objects
	LocationBox = std::make_shared<TransformationWidget>(STransform{{0.0f, 0.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, this);
	LocationBox->CreateWidgets();	
	AddEntity(LocationBox, true);
	LocationBox->SetVisible(false);
	LocationBox->bRayCast = false;
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
	TPointer<Entity> SphereRaycastTest(new Entity(STransform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	TPointer<CSphere> SphereRaycastMesh = std::make_shared<CSphere>(SphereRaycastTest, 2.0f, 2.0f, 2.0f, CliffMaterial);
	SphereRaycastTest->AddMesh(SphereRaycastMesh);
	AddEntity(SphereRaycastTest, true);
			
	TPointer<Entity> FloorEntity(new Entity({{0, 0, 0}, {-90, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	//glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	const TPointer<CPlane> FloorPlanMesh = std::make_shared<CPlane>(FloorEntity, 50.0f, 50.0f, PlaneMaterial);
	FloorEntity->AddMesh(FloorPlanMesh);
	AddEntity(FloorEntity, true);
	
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
	
	TPointer<Entity> CubeEnty(new Entity(STransform{{10.0f, 4.0f, 4.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	TPointer<CCube> CubyMesh(new CCube(CubeEnty, 3.0f, 3.0f, 3.0f, ColouredBrickMaterial));
	CubeEnty->AddMesh(CubyMesh);
	AddEntity(CubeEnty, true);
	
	TPointer<Entity> PyramidEntity(new Entity(STransform{{10.0f, 4.0f, 8.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	TPointer<CPyramid> PyramidMesh(new CPyramid(PyramidEntity, 3.0f, 3.0f, 3.0f, ColouredBrickMaterial));
	PyramidEntity->AddMesh(PyramidMesh);
	// PyramidMesh->SetLit(false, true);
	// TODO: Identify and fix pyramid lighting
	AddEntity(PyramidEntity, true);
	
	TPointer<Entity> SphereEntity(new Entity(STransform{{10.0f, 4.0f, 12.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	TPointer<CSphere> SphereMesh(new CSphere(SphereEntity, 2.0f, 2.0f, 2.0f, ColouredBrickMaterial));
	SphereEntity->AddMesh(SphereMesh);
	AddEntity(SphereEntity, true);
	
	TPointer<Entity> PlaneEntity = std::make_shared<Entity>(STransform{{10.0f, 4.0f, 16.0f}, {-90, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER);
	TPointer<CPlane> PlaneMesh = std::make_shared<CPlane>(PlaneEntity, 2.0f, 2.0f, ColouredBrickPlaneMaterial);
	PlaneEntity->AddMesh(PlaneMesh);
	AddEntity(PlaneEntity, true);
}

void EditorScene::UpdateSelectedEntity()
{
	TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	SVector rayStart = CameraManager::GetInstance()->GetCameraPosition();
	SVector RayDirection = CameraManager::GetInstance()->ScreenToWorldDirection(MousePos);
	SVector HitPos;
	std::vector<TPointer<Entity>> HitEntities;
	std::vector<SVector> HitPosition;
	for (auto& Ent : Entities)
	{
		if (!Ent->bRayCast || !Ent->EntityMesh || !Ent->IsVisible())
		{
			continue; // Don't check for raycast
		}
		// TODO: Fix check hit for plane and pryamid
		if (Ent->CheckHit(rayStart, RayDirection, HitPos))
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
			if (glm::length(rayStart - HitPosition[i]) < glm::length(rayStart - HitPosition[ClosestHitID]))
			{
				ClosestHitID = i;
			}
		}
		TPointer<Entity> HitEntity = HitEntities[ClosestHitID]; // Hit ent
		HitPos = HitPosition[ClosestHitID];

		if (LocationBox)
		{
			LocationBox->Transform.Position = HitEntity->Transform.Position;
			LocationBox->SelectedEntity = HitEntity;
			if (!LocationBox->IsVisible())
			{
				LocationBox->SetVisible(true);
				CurrentFocusDistance = 7.0f;
			}
		}
	}
}

void EditorScene::Update()
{	
	Scene::Update();
	// TODO:
	// LevelNameText->sText = SceneName;
	CameraManager* CameraInstance = CameraManager::GetInstance();
	TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();

	SVector2i ScreenCenter = CameraInstance->GetScreenCenter();
	SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	// TODO:
	const SVector2i ScreenOffset = MousePos - ScreenCenter;
	const SVector2 Offset = SVector2(ScreenOffset) * CameraInstance->MouseSensitivity;
	
	
	
	// TODO: Change later?
	EditorWindowManager::UpdateWindows();
}

void EditorScene::RenderScene(TPointer<IGraphicsInstance> InGraphicsInstance)
{
	Scene::RenderScene(InGraphicsInstance);
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
	
	DestroyEntity(LocationBox);
	DestroyEntity(LocationBox->XMoveTransform);
	DestroyEntity(LocationBox->YMoveTransform);
	DestroyEntity(LocationBox->ZMoveTransform);
	AddEntity(LocationBox);
	AddEntity(LocationBox->XMoveTransform);
	AddEntity(LocationBox->YMoveTransform);
	AddEntity(LocationBox->ZMoveTransform);
	LocationBox->SetActive(true);
	LocationBox->XMoveTransform->SetActive(true);
	LocationBox->YMoveTransform->SetActive(true);
	LocationBox->ZMoveTransform->SetActive(true);
}

void EditorScene::LoadLevel(std::ifstream& OpenedLevelFile)
{
	// std::string LevelName = OpenedLevelFile[0].substr(1, OpenedLevelFile[0].length() - 2);
	// SceneName = LevelName;

	std::vector< TPointer<Entity>> EntitiesCopy = Entities;
	for (TPointer<Entity> CurrentEnt : EntitiesCopy)
	{
		if (CurrentEnt == LocationBox
			|| CurrentEnt == LocationBox->XMoveTransform
			|| CurrentEnt == LocationBox->YMoveTransform
			|| CurrentEnt == LocationBox->ZMoveTransform)
		{
			continue;
		}
		
		DestroyEntity(CurrentEnt);
	}

	std::string Empty;
	std::getline(OpenedLevelFile, Empty, '[');
	std::getline(OpenedLevelFile, SceneName, ']');
	std::getline(OpenedLevelFile, Empty, '\n');
	
	while(OpenedLevelFile.peek() != EOF )
	{
		TPointer<Entity> NewEntity(new Entity(STransform(), EANCHOR::CENTER));
		OpenedLevelFile >> NewEntity;
		std::getline(OpenedLevelFile, Empty, '\n');
		
		TPointer<CMaterial_PBR> TestMaterial = std::make_shared<CMaterial_PBR>("TestMaterial");
		TestMaterial->Params.DiffuseColour = {0.7f, 0.4f, 0.3f, 1.0f};
		
		TPointer<CCube> CubeMesh = std::make_shared<CCube>(NewEntity, 3.0f, 3.0f, 3.0f, TestMaterial);
		NewEntity->AddMesh(CubeMesh);
		NewEntity->SetInitialEntity(true);
		AddEntity(NewEntity, true);
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
	
	DestroyEntity(LocationBox);
	DestroyEntity(LocationBox->XMoveTransform);
	DestroyEntity(LocationBox->YMoveTransform);
	DestroyEntity(LocationBox->ZMoveTransform);
	AddEntity(LocationBox);
	AddEntity(LocationBox->XMoveTransform);
	AddEntity(LocationBox->YMoveTransform);
	AddEntity(LocationBox->ZMoveTransform);
	LocationBox->SetActive(true);
	LocationBox->XMoveTransform->SetActive(true);
	LocationBox->YMoveTransform->SetActive(true);
	LocationBox->ZMoveTransform->SetActive(true);
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
	 	if (Entity == LocationBox
	 		|| Entity == LocationBox->XMoveTransform
	 		|| Entity == LocationBox->YMoveTransform
	 		|| Entity == LocationBox->ZMoveTransform)
	 	{
	 		continue;
	 	}
	 	
	 	LevelFile << Entity << "\n";
	 }

	LevelFile.close();
}

void EditorScene::OnEvent(CEvent& Event)
{
	// TODO: Placeholder as base scene doesn't call derived functions with bind
	EventDispatcher dispatcher(Event);
	dispatcher.Dispatch<CMouseButtonPressedEvent>(SE_BIND_EVENT_FN(EditorScene::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<CMouseButtonReleasedEvent>(SE_BIND_EVENT_FN(EditorScene::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<CMouseMovedEvent>(SE_BIND_EVENT_FN(EditorScene::OnMouseMovedEvent));
	dispatcher.Dispatch<CMouseScrolledEvent>(SE_BIND_EVENT_FN(EditorScene::OnMouseScrolledEvent));
	dispatcher.Dispatch<CKeyPressedEvent>(SE_BIND_EVENT_FN(EditorScene::OnKeyPressedEvent));
	dispatcher.Dispatch<CKeyReleasedEvent>(SE_BIND_EVENT_FN(EditorScene::OnKeyReleasedEvent));
}

bool EditorScene::OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event)
{
	CameraManager* CameraInstance = CameraManager::GetInstance();
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i ScreenCenter = CameraInstance->GetScreenCenter();
	const SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	
	// TODO: Refine mouse visibility toggle/state

	bool bHandled = false;
	if (Event.GetMods() == 0)
	{
		// Right click with no mods
		if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
		{
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorVisible(false);
			CameraInstance->EnableSpectatorControls(true);
			bHandled = true;
		}
		else if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
		{
			UpdateSelectedEntity();
			bHandled = true;
		}
	}

	if (Event.GetMods() & CInput::ModiferType::Alt)
	{
		if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
		{
			bRotatingAroundPoint = true;
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
		}
		else if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
		{
			bLookingAround = true;
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
		}
	}
	if (Event.GetMods() & CInput::ModiferType::Shift)
	{
		if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
		{
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
			bPanning = true;
		}
	}

	bHandled |= bRotatingAroundPoint;
	bHandled |= bLookingAround;
	bHandled |= bPanning;
	if (bHandled)
	{
		return true;
	}
	
	return Scene::OnMouseButtonPressedEvent(Event);
}

bool EditorScene::OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event)
{
	CameraManager* CameraInstance = CameraManager::GetInstance();
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	
	if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
	{
		ApplicationWindow->SetCursorVisible(true);
		CameraInstance->EnableSpectatorControls(false);
		ApplicationWindow->SetCursorPosition(PreviousMousePosition);
		bLookingAround = false;
		return true;
	}
	if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT && (bPanning || bRotatingAroundPoint))
	{
		ApplicationWindow->SetCursorVisible(true);
		ApplicationWindow->SetCursorPosition(PreviousMousePosition);
		bPanning = false;
		bRotatingAroundPoint = false;
		return true;
	}
	if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
	{
		return true;
	}
	return Scene::OnMouseButtonReleasedEvent(Event);
}

bool EditorScene::OnMouseMovedEvent(CMouseMovedEvent& Event)
{
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i ScreenCenter = ApplicationWindow->GetSize() / 2;
	CameraManager* CameraInstance = CameraManager::GetInstance();
	const SVector2i ScreenOffset = Event.GetMousePos() - ScreenCenter;
	const SVector2 Offset = SVector2(ScreenOffset) * CameraInstance->MouseSensitivity;	
	
	const SVector CameraPivotPoint = CameraInstance->GetCameraPosition() + CameraInstance->GetCameraForwardVector() * CurrentFocusDistance;
	if (bRotatingAroundPoint)
	{		
		SVector NewCameraForwardVector = CameraInstance->GetCameraForwardVector();
		NewCameraForwardVector.Rotate(Offset.X, SVector(0,1,0));
		NewCameraForwardVector.Rotate(Offset.Y, CameraInstance->GetCameraRightVector());
		CameraInstance->SetCameraForwardVector(NewCameraForwardVector);
		CameraInstance->SetCameraPos(CameraPivotPoint + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		return true;
	}
	if (bLookingAround)
	{	
		CurrentFocusDistance += Offset.Y * .3f;
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		CameraInstance->SetCameraPos(CameraPivotPoint + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
		return true;
	}
	if (bPanning)
	{		
		SVector NewCameraPosition = CameraInstance->GetCameraPosition();
		NewCameraPosition += CameraInstance->GetCameraRightVector() * Offset.X * CameraInstance->MouseSensitivity;
		NewCameraPosition += -CameraInstance->GetCameraUpVector() * Offset.Y * CameraInstance->MouseSensitivity;
		CameraInstance->SetCameraPos(NewCameraPosition);
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		return true;
	}
	
	return Scene::OnMouseMovedEvent(Event);
}

bool EditorScene::OnMouseScrolledEvent(CMouseScrolledEvent& Event)
{
	return Scene::OnMouseScrolledEvent(Event);
}

bool EditorScene::OnKeyPressedEvent(CKeyPressedEvent& Event)
{
	// if (CInput::GetInstance()->KeyState[GLFW_KEY_ESCAPE] == CInput::INPUT_FIRST_PRESS) // Escape
	// {
	// 	ApplicationWindow->SetCursorVisible(true);
	// 	CameraInstance->EnableSpectatorControls(false);
	// }
	
	if (Event.GetKeyCode() == GLFW_KEY_G)
	{
		Wireframe = !Wireframe;
		const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
		ApplicationWindow->GetGraphicsInstance()->SetWireframeMode(Wireframe);
		return true;
	}
	if (Event.GetKeyCode() == GLFW_KEY_F)
	{
		if (LocationBox && LocationBox->SelectedEntity)
		{
			CameraManager* CameraInstance = CameraManager::GetInstance();
			CameraInstance->SetCameraPos(LocationBox->SelectedEntity->Transform.Position + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
			return true;
		}
	}
	return Scene::OnKeyPressedEvent(Event);
}

bool EditorScene::OnKeyReleasedEvent(CKeyReleasedEvent& Event)
{
	return Scene::OnKeyReleasedEvent(Event);
}
