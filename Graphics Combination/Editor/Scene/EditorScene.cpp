// Copyright Skyward Studios, Inc. All Rights Reserved.

// Library Includes //
#include "EditorScene.h"

// Engine Includes //
#include <fstream>
#include <iostream>

#include "Engine/Camera/Camera.h"
#include "Engine/Input/Input.h"
#include "Engine/UI/UIButton.h"

#include "TransformationWidget.h"
#include "Editor/Windows/EditorWindow.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Engine/Render/Cube.h"

EditorScene::EditorScene(std::string sSceneName) : Scene(sSceneName)
{
	std::shared_ptr<UIButton> QuitBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 20, Camera::GetInstance()->SCR_HEIGHT - 10.0f), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	QuitBtn->AddText("Quit", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, {0, 0});
	QuitBtn->BindPress(this, &EditorScene::QuitCall);
	AddUIElement(QuitBtn);

	std::shared_ptr<UIButton> SaveBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 150, Camera::GetInstance()->SCR_HEIGHT - 10.0f), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	SaveBtn->AddText("Save", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, {0, 0});
	SaveBtn->BindPress(this, &EditorScene::SaveCurrentLevel);
	AddUIElement(SaveBtn);

	std::shared_ptr<UIButton> OpenBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 280, Camera::GetInstance()->SCR_HEIGHT - 10.0f), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 120, 25));
	OpenBtn->AddText("Open", "Resources/Fonts/Roboto-Thin.ttf", 16, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, {0, 0});
	OpenBtn->BindPress(this, &EditorScene::OpenFile);
	AddUIElement(OpenBtn);

	LevelNameText = std::make_shared<UIText>(UIText({Camera::GetInstance()->SCR_WIDTH - 410, Camera::GetInstance()->SCR_HEIGHT - 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "Level Name", "Resources/Fonts/Roboto-Regular.ttf", 20, Utils::BOTTOM_RIGHT));
	AddUIElement(LevelNameText);

	std::shared_ptr<UIText> TipText(new UIText({Camera::GetInstance()->SCR_WIDTH - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, Utils::TOP_RIGHT));
	AddUIElement(TipText);
	
	Camera::GetInstance()->SetCameraPos({-10, 10, 0});
	Camera::GetInstance()->SetCameraForwardVector({0, -0.5, 1.0f});	

	LocationBox = std::make_shared<TransformationWidget>(TransformationWidget(Utils::Transform{{0.0f, 0.0f, 0.0f}, {0, 0, 0}, {1, 1, 1}}, this));
	//std::shared_ptr<Cube> BoxMesh = std::make_shared<Cube>(Cube(0.4f, 0.4f, 0.4f, { 0.9, 0.1, 0.1, 1.0f }));
	//LocationBox->AddMesh(BoxMesh);
	AddEntity(LocationBox, true);
	LocationBox->SetVisible(false, true);
	LocationBox->bRayCast = false;
	
	EditorWindow* NewWindow = new EditorWindow("Outliner", EditorWindowManager::MainWindow, glm::vec2(300, 400), glm::vec2(0, 0));
	NewWindow->SetBackColour(glm::vec3(0.2, 0.6, 0.8));
		
	EditorWindow* ContentWindow = new EditorWindow("Content", EditorWindowManager::MainWindow, glm::vec2(600, 150), glm::vec2(0, Camera::GetInstance()->SCR_WIDTH - 150));
	ContentWindow->SetBackColour(glm::vec3(0.4, 0.4, 0.4));

	//EditorWindow* ExternalWindow = new EditorWindow("External Test", nullptr, glm::vec2(500, 300), glm::vec2(100, 100));
	//ExternalWindow->SetBackColour(glm::vec3(0.6, 0.3, 0.4));

}

void EditorScene::Update()
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

	// TODO: Change later?
	EditorWindowManager::UpdateWindows();
}

void EditorScene::OpenFile()
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

void EditorScene::LoadLevel(std::vector<std::string> Lines)
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

void EditorScene::SaveCurrentLevel()
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

void EditorScene::QuitCall()
{
	glfwSetWindowShouldClose(EditorWindowManager::MainWindow, true);
}