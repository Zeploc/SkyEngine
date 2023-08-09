// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "EditorWindow.h"

#include "EditorWindowManager.h"
#include <iostream>

#include <Camera/CameraManager.h>
#include <Input/Input.h>
#include <System/Utils.h>
#include <UI/Legacy/UIButton.h>

#include "Graphics/GraphicsInstance.h"
#include "Platform/Window/EngineWindow.h"

void EditorWindow::CreateExternalWindow()
{
	LinkedWindow = CEngineWindow::CreateEngineWindow(WindowName, Size, false);
	LinkedWindow->SetWindowPosition(Position);
}

EditorWindow::EditorWindow(std::string InWindowName, TPointer<CEngineWindow> InLinkedWindow, SVector2 InSize, SVector2 InPosition)
: WindowName(InWindowName), LinkedWindow(InLinkedWindow), Size(InSize), Position(InPosition)
{
	if (!LinkedWindow)
	{
		CreateExternalWindow();
	}
	
	EditorWindowManager::NewWindowCreated(this);
	
	SetupUI();
}

void EditorWindow::SetupGlutBindings()
{
	//glutDisplayFunc(*EditorWindowManager::StaticRenderWindow);
}

void EditorWindow::SetupUI()
{
	if (LinkedWindow == EditorWindowManager::GetMainWindow())
	{
		BackImage = std::make_shared<UIImage>(UIImage(glm::vec2(Size.X / 2.0f, Size.Y / 2.0f), EANCHOR::CENTER, 0.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), Size.X, Size.Y));
		//TestBtn->AddText("Test", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
		UIElements.push_back(BackImage);

		//20, Size.y - 20.0f
		TPointer<UIButton> TestBtn(new UIButton(glm::vec2(0.0f, 0.0f), EANCHOR::TOP_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), Size.X - 35, 30, nullptr));

		TestBtn->BindPress(this, &EditorWindow::StartDrag);
		TestBtn->BindRelease(this, &EditorWindow::StopDrag);

		TestBtn->AddText(WindowName, "Resources/Fonts/Roboto-Thin.ttf", 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
		UIElements.push_back(TestBtn);

		TPointer<UIButton> PopoutButton(new UIButton(glm::vec2(Size.X, 0.0f), EANCHOR::TOP_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 30, 30, nullptr));

		PopoutButton->BindPress(this, &EditorWindow::PopOut);
		UIElements.push_back(PopoutButton);
	}
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::RenderWindow()
{	
	if (LinkedWindow != EditorWindowManager::GetMainWindow()) // Separate window
	{
		LinkedWindow->GetGraphicsInstance()->ClearColour = BackColour;
		LinkedWindow->PreRender();
	}
	else
	{
		// Position viewport within existing main window
		const SVector2i MainWindowSize = EditorWindowManager::GetMainWindow()->GetSize();
		LinkedWindow->GetGraphicsInstance()->SetRenderViewport({Position.X, MainWindowSize.Y - Position.Y - Size.Y}, {Size.X, Size.Y});
	}

	LinkedWindow->Render({}, UIElements);
	
	if (LinkedWindow != EditorWindowManager::GetMainWindow()) // Separate window
	{
		LinkedWindow->PostRender();
	}
}

void EditorWindow::PopOut()
{
	//EditorWindowManager::WindowRemoved(this);

	CreateExternalWindow();
	
	if (LinkedWindow == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
		return;
	}

	const SVector2i MainWindowPosition = EditorWindowManager::GetMainWindow()->GetPosition();
	Position.Y += 30;
	LinkedWindow->SetWindowPosition(MainWindowPosition + Position);	

	// FORCE SINCE RENDER DOESN'T WORK AFTER THIS LOL
	UIElements.clear();

	//EditorWindowManager::NewWindowCreated(this);

	bCanPopIn = false;

	//glutSetWindow(EditorWindowManager::iMainWindowID);
	//int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
	//int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);

	//glutDestroyWindow(WindowID);
	//EditorWindowManager::WindowRemoved(WindowID);

	//glm::vec2 NewWindowPos = Position;
	//NewWindowPos.x += screen_pos_x;
	//NewWindowPos.y += screen_pos_y;

	//glutInitWindowPosition(NewWindowPos.x, NewWindowPos.y);
	//glutInitWindowSize(Size.x, Size.y);
	//WindowID = glutCreateWindow(WindowName.c_str());
	//SetupGlutBindings();
	//EditorWindowManager::NewWindowCreated(this);
	//CameraManager::GetInstance()->Init(Size.x, Size.y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
	//CameraManager::GetInstance()->SwitchProjection(CameraManager::Perspective);
}

void EditorWindow::PopIn()
{
	if (!bCanPopIn)
	{
		return;
	}
	if (LinkedWindow == EditorWindowManager::GetMainWindow()) //Same window
	{
		return;
	}

	LinkedWindow.reset();
	LinkedWindow = EditorWindowManager::GetMainWindow();

	const SVector2i MainWindowPosition = LinkedWindow->GetPosition();
	Position -= MainWindowPosition;
	Position.Y -= 30;

	SetupUI();
	SetBackColour(BackColour);
}

void EditorWindow::StartDrag()
{
	// DragOffset = CInput::GetInstance()->MousePos - Position;
	DraggingWindow = true;
}

void EditorWindow::StopDrag()
{
	if (DraggingWindow)
	{
		// SVector2i NewPosition = CInput::GetInstance()->MousePos - DragOffset;
		// SetWindowPosition(NewPosition);

		DraggingWindow = false;
	}
}

bool EditorWindow::IsPointInWindow(SVector2i _point)
{
	if (_point.X < Position.X || _point.Y < Position.Y)
	{
		return false;
	}

	const SVector2i EndPos = Position + Size;

	if (_point.X > EndPos.X || _point.Y > EndPos.Y)
	{
		return false;
	}
	return true;
}

void EditorWindow::SetBackColour(glm::vec3 _Colour)
{
	BackColour = _Colour;
	if (BackImage)
	{
		BackImage->Colour = glm::vec4(BackColour, 1.0f);
	}
}

void EditorWindow::UpdateWindow()
{
	if (LinkedWindow != EditorWindowManager::GetMainWindow()) //seperate window
	{		
		const SVector2i MainWindowPosition = EditorWindowManager::GetMainWindow()->GetPosition();
		const SVector2i MainWindowSize = EditorWindowManager::GetMainWindow()->GetSize();
		
		// Update position
		GetPosition();

		if (Position.X + Size.X > MainWindowPosition.X && Position.X < MainWindowPosition.X + MainWindowSize.X &&
			Position.Y > MainWindowPosition.Y && Position.Y < MainWindowPosition.Y + MainWindowSize.Y)
		{
			std::cout << "Window Pos: " << Position.ToString() << std::endl;
			PopIn();
		}
		else
		{
			bCanPopIn = true;
		}
	}

	LinkedWindow->FocusWindow();

	if (DraggingWindow)
	{
		// SVector2i NewPosition = CInput::GetInstance()->MousePos - DragOffset;
		// SetWindowPosition(NewPosition);
	}

	//glm::vec2 MousePosViewport = CInput::GetInstance()->MousePos;
	////MousePosViewport += ViewportOffset;
	//glm::vec2 TopLeft = Position;
	//glm::vec2 BottomRight = Position + Size;
	//bool InWindow = (MousePosViewport.x > TopLeft.x && MousePosViewport.x < BottomRight.x && MousePosViewport.y < BottomRight.y && MousePosViewport.y > TopLeft.y);
	//
	//if (CInput::GetInstance()->MouseState[CInput::MOUSE_LEFT] == CInput::InputState::INPUT_FIRST_PRESS && InWindow)
	//{
	//	DragOffset = CInput::GetInstance()->MousePos - Position;
	//	DraggingWindow = true;
	//	//PopOut();
	//}
	//else if (CInput::GetInstance()->MouseState[CInput::MOUSE_LEFT] == CInput::InputState::INPUT_HOLD && DraggingWindow)
	//{
	//	glm::vec2 NewPosition = CInput::GetInstance()->MousePos - DragOffset;
	//	SetWindowPosition(NewPosition);
	//}
	//else if(CInput::GetInstance()->MouseState[CInput::MOUSE_LEFT] == CInput::InputState::INPUT_FIRST_RELEASE)
	//{
	//	if (DraggingWindow)
	//	{
	//		glm::vec2 NewPosition = CInput::GetInstance()->MousePos - DragOffset;
	//		SetWindowPosition(NewPosition);

	//		DraggingWindow = false;
	//	}
	//	
	//}

	for (TPointer<UIElement> UIElement : UIElements)
	{
		UIElement->BaseUpdate();
	}

	//CInput::GetInstance()->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
}

SVector2i EditorWindow::GetPosition()
{
	if (LinkedWindow != EditorWindowManager::GetMainWindow())
	{
		Position = LinkedWindow->GetPosition();
	}
	return Position;
}

void EditorWindow::SetWindowPosition(SVector2i _position)
{
	if (LinkedWindow != EditorWindowManager::GetMainWindow())
	{
		LinkedWindow->SetWindowPosition(_position);
	}
	Position = _position;
}

void EditorWindow::MainWindowSizeChanged(int _w, int _h)
{
	if (LinkedWindow != EditorWindowManager::GetMainWindow()) //seperate window
	{
		Size = glm::tvec2<int>(_w, _h);
	}
	//glutSetWindow(WindowID);

	//CameraManager::GetInstance()->SCR_HEIGHT = _h;
	//CameraManager::GetInstance()->SCR_WIDTH = _w;

	// resize and reposition the sub window
	//glutPositionWindow(border, (h + border) / 2);
	//glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
}
