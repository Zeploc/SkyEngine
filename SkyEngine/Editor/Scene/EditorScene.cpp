// Copyright Skyward Studios, Inc. All Rights Reserved.

// Library Includes //
#include "EditorScene.h"

// Engine Includes //
#include <fstream>
#include <iostream>

#include "Engine/Camera/CameraManager.h"
#include "Engine/Input/Input.h"
#include "Engine/UI/UIText.h"
#include "Engine/UI/UIButton.h"

#include "TransformationWidget.h"
#include "Editor/Windows/EditorWindow.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Engine/Render/Cube.h"
#include "Engine/Render/GeometryObject.h"
#include "Engine/Render/Model.h"
#include "Engine/Render/ParticleSystem.h"
#include "Engine/Render/Plane.h"
#include "Engine/Render/Pyramid.h"
#include "Engine/Render/Sphere.h"

#include <Windows.h>

EditorScene::EditorScene(std::string sSceneName) : Scene(sSceneName)
{
	std::shared_ptr<UIButton> QuitBtn(new UIButton(glm::vec2(CameraManager::GetInstance()->SCR_WIDTH - 20, CameraManager::GetInstance()->SCR_HEIGHT - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	QuitBtn->AddText("Quit", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	// TODO: Let delegate allow derived class type when passing in "this"? - Auto cast check inside? Template type for derived types allowed?
	QuitBtn->BindPress(static_cast<Scene*>(this), &Scene::QuitApplication);
	AddUIElement(QuitBtn);

	std::shared_ptr<UIButton> SaveBtn(new UIButton(glm::vec2(CameraManager::GetInstance()->SCR_WIDTH - 150, CameraManager::GetInstance()->SCR_HEIGHT - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	SaveBtn->AddText("Save", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	SaveBtn->BindPress(this, &EditorScene::SaveCurrentLevel);
	AddUIElement(SaveBtn);
	
	std::shared_ptr<UIButton> SaveAsNewBtn(new UIButton(glm::vec2(CameraManager::GetInstance()->SCR_WIDTH - 280, CameraManager::GetInstance()->SCR_HEIGHT - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	SaveAsNewBtn->AddText("Save As", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	SaveAsNewBtn->BindPress(this, &EditorScene::SaveAsNew);
	AddUIElement(SaveAsNewBtn);

	std::shared_ptr<UIButton> OpenBtn(new UIButton(glm::vec2(CameraManager::GetInstance()->SCR_WIDTH - 410, CameraManager::GetInstance()->SCR_HEIGHT - 10.0f), EANCHOR::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	OpenBtn->AddText("Open", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
	OpenBtn->BindPress(this, &EditorScene::OpenFile);
	AddUIElement(OpenBtn);

	LevelNameText = std::make_shared<UIText>(glm::vec2(CameraManager::GetInstance()->SCR_WIDTH - 540, CameraManager::GetInstance()->SCR_HEIGHT - 15.0f), 0.0f, glm::vec4(0.3, 0.3, 0.3, 1.0f), "Level Name", "Resources/Fonts/Roboto-Regular.ttf", 20, EANCHOR::BOTTOM_RIGHT);
	AddUIElement(LevelNameText);

	std::shared_ptr<UIText> TipText(new UIText({CameraManager::GetInstance()->SCR_WIDTH - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, EANCHOR::TOP_RIGHT));
	AddUIElement(TipText);
	
	Lighting::m_v3LightPosition = {5, 7, 5};
	Lighting::m_v3SunDirection = {3, -1, 5};
	
	CameraManager::GetInstance()->SetCameraPos({-10, 10, 0});
	CameraManager::GetInstance()->SetCameraForwardVector({0, -0.5, 1.0f});	
	
	EditorWindow* NewWindow = new EditorWindow("Outliner", EditorWindowManager::MainWindow, glm::vec2(300, 400), glm::vec2(0, 0));
	NewWindow->SetBackColour(glm::vec3(0.2, 0.6, 0.8));
		
	EditorWindow* ContentWindow = new EditorWindow("Content", EditorWindowManager::MainWindow, glm::vec2(600, 150), glm::vec2(0, CameraManager::GetInstance()->SCR_HEIGHT - 150));
	ContentWindow->SetBackColour(glm::vec3(0.4, 0.4, 0.4));

	//EditorWindow* ExternalWindow = new EditorWindow("External Test", nullptr, glm::vec2(500, 300), glm::vec2(100, 100));
	//ExternalWindow->SetBackColour(glm::vec3(0.6, 0.3, 0.4));

	AddSampleEntities();
	
	std::shared_ptr<Entity> CubeEnty(new Entity(FTransform{{10.0f, 4.0f, 4.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Cube> CubyMesh(new Cube(3.0f, 3.0f, 3.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	CubeEnty->AddMesh(CubyMesh);
	CubyMesh->SetLit(true);
	// CubyMesh->SetReflection(true);
	AddEntity(CubeEnty, true);

	// Last added to appear on top
	// TODO: Depth test order to always be infront of loaded level objects
	LocationBox = std::make_shared<TransformationWidget>(FTransform{{0.0f, 0.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, this);
	AddEntity(LocationBox, true);
	LocationBox->SetVisible(false);
	LocationBox->bRayCast = false;
}

void EditorScene::AddSampleEntities()
{
	std::shared_ptr<Entity> SphereRaycastTest(new Entity(FTransform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Sphere> SphereRaycastMesh(new Sphere(2.0f, 2.0f, 2.0f, {0.1f, 0.8f, 0.3f, 1.0f}, "Resources/Images/SmoothCliff_1024.jpg"));
	SphereRaycastTest->AddMesh(SphereRaycastMesh);
	SphereRaycastMesh->SetLit(true);
	SphereRaycastMesh->SetReflection(true);
	AddEntity(SphereRaycastTest, true);
	
	std::shared_ptr<Entity> FloorEntity(new Entity({{0, 0, 0}, {90, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	//glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	std::shared_ptr<Plane> FloorPlanMesh(new Plane(50, 50, {0.5f, 0.5f, 0.5f, 1.0f}));
	FloorEntity->AddMesh(FloorPlanMesh);
	FloorPlanMesh->bCullFace = false;
	AddEntity(FloorEntity, true);

	std::shared_ptr<ParticleSystem> ParticleBoy(new ParticleSystem({{20, 8, 10}, {0, 0, 0}, {1, 1, 1}}));
	ParticleBoy->SetPositionRange({-5, 5}, {0, 0}, {-5, 5});
	ParticleBoy->SetDirectionRange({0, 0}, {-1, -1}, {0, 0});
	ParticleBoy->SetFalloffTime({100.0f, 100.0f});
	ParticleBoy->SetFalloffRange({30, 40});
	ParticleBoy->SetSpeedRange({1, 10});
	ParticleBoy->ParticleSize = 0.8f;
	ParticleBoy->Init(1000, "Resources/Images/raindrop.png");
	AddEntity(ParticleBoy, true);
	
	std::shared_ptr<ParticleSystem> ParticleBoy2(new ParticleSystem({{20, 1, 20}, {0, 0, 0}, {1, 1, 1}}));
	ParticleBoy2->Init(1000, "Resources/Images/Box.png");
	AddEntity(ParticleBoy2, true);
	
	std::shared_ptr<Entity> GeomEnt(new Entity({{10, 6, 10}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<GeometryObject> GeomShape(new GeometryObject({0.0, 0.9f, 0.3f, 1.0f}));
	GeomEnt->AddMesh(GeomShape);
	AddEntity(GeomEnt, true);
	
	std::shared_ptr<Entity> ModelEntity(new Entity(FTransform{{10.0f, 2.0f, 0.0f}, {-90, -90, 0}, {0.2f, 0.2f, 0.2f}}, EANCHOR::CENTER));
	std::shared_ptr<Model> ModelEntityMesh(new Model({1.0f, 1.0f, 1.0f, 1.0f}, "Resources/Models/theDude.DAE"));
	ModelEntity->AddMesh(ModelEntityMesh);
	ModelEntityMesh->SetLit(true);
	ModelEntityMesh->SetReflection(true);
	AddEntity(ModelEntity, true);
	
	std::shared_ptr<Entity> CubeEnty(new Entity(FTransform{{10.0f, 4.0f, 4.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Cube> CubyMesh(new Cube(3.0f, 3.0f, 3.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	CubeEnty->AddMesh(CubyMesh);
	CubyMesh->SetLit(true);
	CubyMesh->SetReflection(true);
	AddEntity(CubeEnty, true);
	
	std::shared_ptr<Entity> PyramidEntity(new Entity(FTransform{{10.0f, 4.0f, 8.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Pyramid> PyramidMesh(new Pyramid(3.0f, 3.0f, 3.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	PyramidEntity->AddMesh(PyramidMesh);
	// PyramidMesh->SetLit(false, true);
	// TODO: Identify and fix pyramid lighting
	PyramidMesh->SetLit(true);
	PyramidMesh->SetReflection(true);
	AddEntity(PyramidEntity, true);
	
	std::shared_ptr<Entity> SphereEntity(new Entity(FTransform{{10.0f, 4.0f, 12.0f}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Sphere> SphereMesh(new Sphere(2.0f, 2.0f, 2.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	SphereEntity->AddMesh(SphereMesh);
	SphereMesh->SetLit(true);
	SphereMesh->SetReflection(true);
	AddEntity(SphereEntity, true);
	
	std::shared_ptr<Entity> PlaneEntity(new Entity(FTransform{{10.0f, 4.0f, 16.0f}, {-90, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	std::shared_ptr<Plane> PlaneMesh(new Plane(2.0f, 2.0f, {0.5f, 0.3f, 0.3f, 1.0f}, "Resources/Images/StoneWall_2x2.jpg"));
	PlaneEntity->AddMesh(PlaneMesh);
	PlaneMesh->SetLit(true);
	PlaneMesh->SetReflection(true);
	PlaneMesh->bCullFace = false;
	AddEntity(PlaneEntity, true);
}

void EditorScene::UpdateSelectedEntity()
{
	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_HOLD)
	{
		Vector3 rayStart = CameraManager::GetInstance()->GetCameraPosition();
		Vector3 RayDirection = CameraManager::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);
		Vector3 HitPos;
		std::vector<std::shared_ptr<Entity>> HitEntities;
		std::vector<Vector3> HitPosition;
		for (auto& Ent : Entities)
		{
			if (!Ent->bRayCast || !Ent->EntityMesh || !Ent->IsVisible())
			{
				continue; // Don't check for raycast
			}
			// TODO: Fix check hit for plane and pryamid
			if (Utils::CheckHit(rayStart, RayDirection, Ent, HitPos))
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
				if ((rayStart - HitPosition[i]).Size() < (rayStart - HitPosition[ClosestHitID]).Size())
				{
					ClosestHitID = i;
				}
			}
			std::shared_ptr<Entity> HitEntity = HitEntities[ClosestHitID]; // Hit ent
			HitPos = HitPosition[ClosestHitID];

			if (LocationBox && Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS)
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
}

void EditorScene::Update()
{	
	Scene::Update();
	LevelNameText->sText = SceneName;
	CameraManager* CameraInstance = CameraManager::GetInstance();

	// TODO: Change from moving back to center to instead get offset (loop mouse when edge of screen?
	Vector2 CenterScreen(static_cast<double>(CameraInstance->SCR_WIDTH) * 0.5, static_cast<double>(CameraInstance->SCR_HEIGHT) * 0.5);	
	
	Vector2 Offset = Input::GetInstance()->MousePos - CenterScreen;
	Offset *= CameraInstance->MouseSensitivity;
	
	const bool bAltDown = Input::GetInstance()->MouseALT == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseALT == Input::INPUT_HOLD;
	const bool bShiftDown = Input::GetInstance()->MouseSHIFT == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseSHIFT == Input::INPUT_HOLD;

	// TODO: Refine mouse visibility toggle/state
	
	if (Input::GetInstance()->KeyState[GLFW_KEY_ESCAPE] == Input::INPUT_FIRST_PRESS) // Escape
	{
		//Input::GetInstance()->ToggleCursorVisible();
		//CameraManager::GetInstance()->EnableSpectatorControls(!Input::GetInstance()->GetCursorVisible());

		Input::GetInstance()->SetCursorVisible(true);
		CameraInstance->EnableSpectatorControls(false);
	}
	if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_FIRST_PRESS && !bAltDown && !bShiftDown)
	{
		PreviousMousePosition = Input::GetInstance()->MousePos;
		Input::GetInstance()->SetCursorVisible(false);
		CameraInstance->EnableSpectatorControls(true);
	}
	else if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_FIRST_RELEASE)
	{
		Input::GetInstance()->SetCursorVisible(true);
		CameraInstance->EnableSpectatorControls(false);
		// glfwSetCursorPos(CameraInstance->MainWindow, PreviousMousePosition.X, PreviousMousePosition.Y);
	}
	if (Input::GetInstance()->KeyState[GLFW_KEY_G] == Input::INPUT_FIRST_PRESS)
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
	if (Input::GetInstance()->KeyState[GLFW_KEY_F] == Input::INPUT_FIRST_PRESS)
	{
		if (LocationBox && LocationBox->SelectedEntity)
		{
			CameraInstance->SetCameraPos(LocationBox->SelectedEntity->Transform.Position + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
		}
	}

	// TODO: Change spectator to use left click while alt down
	if (bAltDown)
	{
		const Vector3 CameraPivotPoint = CameraInstance->GetCameraPosition() + CameraInstance->GetCameraForwardVector() * CurrentFocusDistance;
		if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_HOLD)
		{
			if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_FIRST_PRESS)
			{
				Input::GetInstance()->SetCursorVisible(false);
			}
			else
			{
				Vector3 NewCameraForwardVector = CameraInstance->GetCameraForwardVector();
				NewCameraForwardVector.Rotate(-Offset.X, Vector3(0,1,0));
				NewCameraForwardVector.Rotate(-Offset.Y, CameraInstance->GetCameraRightVector());
				CameraInstance->SetCameraForwardVector(NewCameraForwardVector);
			}
			PreviousMousePosition = Input::GetInstance()->MousePos;
			glfwSetCursorPos(CameraInstance->MainWindow, CenterScreen.X, CenterScreen.Y);
		}
		else if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_HOLD)
		{
			if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_RIGHT] == Input::INPUT_FIRST_PRESS)
			{
				Input::GetInstance()->SetCursorVisible(false);
			}
			else
			{
				CurrentFocusDistance += Offset.Y * .3f;
			}
			PreviousMousePosition = Input::GetInstance()->MousePos;
			glfwSetCursorPos(CameraInstance->MainWindow, CenterScreen.X, CenterScreen.Y);
		}
		else if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_FIRST_RELEASE)
		{
			Input::GetInstance()->SetCursorVisible(true);
		}
		CameraInstance->SetCameraPos(CameraPivotPoint + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
	}
	// TODO: Refine release of alt
	else if (Input::GetInstance()->KeyState[GLFW_KEY_LEFT_ALT] == Input::INPUT_FIRST_RELEASE)
	{
		Input::GetInstance()->SetCursorVisible(true);
		// glfwSetCursorPos(CameraInstance->MainWindow, PreviousMousePosition.X, PreviousMousePosition.Y);
	}
	if (bShiftDown)
	{
		if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_HOLD)
		{
			if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_FIRST_PRESS)
			{
				Input::GetInstance()->SetCursorVisible(false);
			}
			else
			{
				Vector3 NewCameraPosition = CameraInstance->GetCameraPosition();
				NewCameraPosition += -CameraInstance->GetCameraRightVector() * Offset.X * CameraInstance->MouseSensitivity;
				NewCameraPosition += CameraInstance->GetCameraUpVector() * Offset.Y * CameraInstance->MouseSensitivity;
				CameraInstance->SetCameraPos(NewCameraPosition);
			}
			PreviousMousePosition = Input::GetInstance()->MousePos;
			glfwSetCursorPos(CameraInstance->MainWindow, CenterScreen.X, CenterScreen.Y);
		}
		else if (Input::GetInstance()->MouseState[GLFW_MOUSE_BUTTON_LEFT] == Input::INPUT_FIRST_RELEASE)
		{
			Input::GetInstance()->SetCursorVisible(true);
			// glfwSetCursorPos(CameraInstance->MainWindow, PreviousMousePosition.X, PreviousMousePosition.Y);
		}
	}

	if (!bAltDown && !bShiftDown)
	{
		UpdateSelectedEntity();
	}
	
	// TODO: Change later?
	EditorWindowManager::UpdateWindows();
}

void EditorScene::RenderScene()
{
	Scene::RenderScene();

	EditorWindowManager::RenderWindows();	
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
		MessageBox(NULL, FileName.c_str(), "Failed to open", MB_OK);
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

	std::vector< std::shared_ptr<Entity>> EntitiesCopy = Entities;
	for (std::shared_ptr<Entity> CurrentEnt : EntitiesCopy)
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
		std::shared_ptr<Entity> NewEntity(new Entity(FTransform(), EANCHOR::CENTER));
		OpenedLevelFile >> NewEntity;
		std::getline(OpenedLevelFile, Empty, '\n');
		
		std::shared_ptr<Cube> CubeMesh(new Cube(3.0f, 3.0f, 3.0f, {0.7f, 0.4f, 0.3f, 1.0f}));
		NewEntity->AddMesh(CubeMesh);
		NewEntity->SetInitialEntity(true);
		AddEntity(NewEntity, true);
	}
	
	// for (std::string Line : OpenedLevelFile)
	// {
	// 	std::string IsEntity = Line.substr(0, 8);
	// 	if (IsEntity == "[Entity]")
	// 	{
	// 		// std::shared_ptr<Entity> NewEntity;
	// 		// IsEntity >> NewEntity;
	// 		
	// 		std::shared_ptr<Entity> CubeEnty(new Entity(Line));
	// 		//std::shared_ptr<Sphere> SphereMesh(Sphere(2.0f, 2.0f, 2.0f, { 0.1f, 0.8f, 0.3f, 1.0f }));
	// 		std::shared_ptr<Cube> CubyMesh(new Cube(3.0f, 3.0f, 3.0f, {0.7f, 0.4f, 0.3f, 1.0f}));
	//
	// 		CubeEnty->AddMesh(CubyMesh);
	// 		CubeEnty->SetInitialEntity(true);
	// 		
	// 		//std::shared_ptr<GeometryObject> GeomShape(new GeometryObject({0.0, 0.9f, 0.3f, 1.0f}));
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
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.slvl)\0*.slvl\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = const_cast<LPSTR>("Untitled.slvl");
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "Resources/Levels/";
	ofn.Flags = OFN_PATHMUSTEXIST;// | OFN_FILEMUSTEXIST;
	if (!GetSaveFileNameA(&ofn))
	{
		// MessageBox(NULL, ofn.lpstrFile, "File Name", MB_OK);
		return;
	}
	MessageBox(NULL, ofn.lpstrFile, "File Name (WIP)", MB_OK);
}

void EditorScene::SaveCurrentLevel()
{
	std::ofstream LevelFile;
	std::string LevelPath = "Resources/Levels/" + SceneName + ".slvl";
	LevelFile.open(LevelPath);
	if (!LevelFile.is_open())
    {
    	MessageBox(NULL, LevelPath.c_str(), "Failed to edit", MB_OK);
    	return;
    }
	LevelFile << "[" + SceneName + "]\n";
	for (std::shared_ptr<Entity> Entity : Entities)
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
