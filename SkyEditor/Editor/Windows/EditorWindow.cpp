// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "EditorWindow.h"

#include "EditorWindowManager.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Camera/CameraManager.h>
#include <Input/Input.h>
#include <Render/Shader.h>
#include <System/Utils.h>
#include <UI/UIButton.h>

EditorWindow::EditorWindow(std::string _WindowName, GLFWwindow* _ParentWindow, glm::vec2 _Size, glm::vec2 _Position) : WindowName(_WindowName), ParentWindow(_ParentWindow), Size(_Size), Position(_Position)
{
	// TODO: Fix errors
	// if (!_ParentWindow)
	// {
	// 	ParentWindow = glfwCreateWindow(Size.X, Size.Y, WindowName.c_str(), nullptr, nullptr);
	// 	if (ParentWindow == nullptr)
	// 	{
	// 		std::cout << "Failed to create GLFW window" << std::endl;
	// 		return;
	// 	}
	// 	glfwMakeContextCurrent(ParentWindow);
	//
	// 	glfwSetWindowPos(ParentWindow, Position.X, Position.Y);
	//
	// 	glViewport(0, 0, Size.X, Size.Y);
	//
	// 	// OpenGL init
	// 	glewInit();
	//
	// 	// Settings Initialised
	// 	Shader::LoadAllDefaultShadersInCurrentContext();
	//
	// 	glCullFace(GL_BACK); // Cull the Back faces
	// 	glFrontFace(GL_CW); // Front face is Clockwise order
	// 	glEnable(GL_CULL_FACE); // Turn on the back face culling
	//
	// 	glEnable(GL_BLEND);
	// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//
	// 	// The input function registration
	// 	Input::GetInstance()->Init(ParentWindow);
	// }
	// else
	// {
	// }
	//
	// EditorWindowManager::NewWindowCreated(this);
	//
	// SetupUI();
	//
	// glfwMakeContextCurrent(EditorWindowManager::MainWindow);
}

void EditorWindow::SetupGlutBindings()
{
	//glutDisplayFunc(*EditorWindowManager::StaticRenderWindow);
}

void EditorWindow::SetupUI()
{
	if (ParentWindow == EditorWindowManager::MainWindow)
	{
		BackImage = std::make_shared<UIImage>(UIImage(glm::vec2(Size.X / 2.0f, Size.Y / 2.0f), EANCHOR::CENTER, 0.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), Size.X, Size.Y));
		//TestBtn->AddText("Test", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
		UIElements.push_back(BackImage);

		//20, Size.y - 20.0f
		std::shared_ptr<UIButton> TestBtn(new UIButton(glm::vec2(0.0f, 0.0f), EANCHOR::TOP_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), Size.X - 35, 30, nullptr));

		TestBtn->BindPress(this, &EditorWindow::StartDrag);
		TestBtn->BindRelease(this, &EditorWindow::StopDrag);

		TestBtn->AddText(WindowName, "Resources/Fonts/Roboto-Thin.ttf", 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), EANCHOR::CENTER, {0, 0});
		UIElements.push_back(TestBtn);

		std::shared_ptr<UIButton> PopoutButton(new UIButton(glm::vec2(Size.X, 0.0f), EANCHOR::TOP_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 30, 30, nullptr));

		PopoutButton->BindPress(this, &EditorWindow::PopOut);
		UIElements.push_back(PopoutButton);
	}
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::RenderWindow()
{
	if (ParentWindow != EditorWindowManager::MainWindow) //seperate window
	{
		glfwMakeContextCurrent(ParentWindow);
		glClearColor(BackColour.r, BackColour.g, BackColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	//glfwMakeContextCurrent(ParentWindow);

	int width, height;
	glfwGetWindowSize(ParentWindow, &width, &height);

	glViewport(Position.X, height - Position.Y - Size.Y, Size.X, Size.Y);
	CameraManager::GetInstance()->SCR_WIDTH = Size.X;
	CameraManager::GetInstance()->SCR_HEIGHT = Size.Y;
	CameraManager::GetInstance()->VIEWPORT_X = Position.X;
	CameraManager::GetInstance()->VIEWPORT_Y = Position.Y;

	glDisable(GL_DEPTH_TEST);

	for (std::shared_ptr<UIElement> UIElement : UIElements)
	{
		UIElement->DrawUIElement();
	}
	if (ParentWindow != EditorWindowManager::MainWindow) //seperate window
	{
		glfwSwapBuffers(ParentWindow);
		glfwMakeContextCurrent(EditorWindowManager::MainWindow);
	}
}

void EditorWindow::PopOut()
{
	// TODO: Fix
	/*//EditorWindowManager::WindowRemoved(this);

	ParentWindow = glfwCreateWindow(Size.X, Size.Y, WindowName.c_str(), nullptr, nullptr);
	if (ParentWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return;
	}
	glfwMakeContextCurrent(ParentWindow);

	int x, y;
	glfwGetWindowPos(EditorWindowManager::MainWindow, &x, &y);

	Position.Y += 30;
	glfwSetWindowPos(ParentWindow, x + Position.X, y + Position.Y);

	glViewport(0, 0, Size.X, Size.Y);

	// OpenGL init
	glewInit();

	// Settings Initialised
	Shader::LoadAllDefaultShadersInCurrentContext();

	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// The input function registration
	Input::GetInstance()->Init(ParentWindow);

	// FORCE SINCE RENDER DOESN'T WORK AFTER THIS LOL
	UIElements.clear();

	//EditorWindowManager::NewWindowCreated(this);

	glfwMakeContextCurrent(EditorWindowManager::MainWindow);

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
	//CameraManager::GetInstance()->SwitchProjection(CameraManager::Perspective);*/
}

void EditorWindow::PopIn()
{
	if (!bCanPopIn)
	{
		return;
	}
	if (ParentWindow == EditorWindowManager::MainWindow) //Same window
	{
		return;
	}

	glfwDestroyWindow(ParentWindow);
	ParentWindow = EditorWindowManager::MainWindow;

	int MainX, MainY;
	glfwGetWindowPos(ParentWindow, &MainX, &MainY);
	Position -= glm::vec2(MainX, MainY);
	Position.Y -= 30;

	SetupUI();
	SetBackColour(BackColour);
}

void EditorWindow::StartDrag()
{
	DragOffset = Input::GetInstance()->MousePos - Position;
	DraggingWindow = true;
}

void EditorWindow::StopDrag()
{
	if (DraggingWindow)
	{
		Vector2 NewPosition = Input::GetInstance()->MousePos - DragOffset;
		SetWindowPosition(NewPosition);

		DraggingWindow = false;
	}
}

bool EditorWindow::IsPointInWindow(Vector2 _point)
{
	if (_point.X < Position.X || _point.Y < Position.Y)
	{
		return false;
	}

	Vector2 EndPos = Position + Size;

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
	if (ParentWindow != EditorWindowManager::MainWindow) //seperate window
	{
		int MainX, MainY, MainWidth, MainHeight;
		glfwGetWindowPos(EditorWindowManager::MainWindow, &MainX, &MainY);
		glfwGetWindowSize(EditorWindowManager::MainWindow, &MainWidth, &MainHeight);
		// Update position
		GetPosition();

		if (Position.X + Size.X > MainX && Position.X < MainX + MainWidth && Position.Y > MainY && Position.Y < MainY + MainHeight)
		{
			std::cout << "Window Pos: " << Position.ToString() << std::endl;
			PopIn();
		}
		else
		{
			bCanPopIn = true;
		}
	}

	glfwMakeContextCurrent(ParentWindow);

	CameraManager::GetInstance()->SCR_WIDTH = Size.X;
	CameraManager::GetInstance()->SCR_HEIGHT = Size.Y;
	CameraManager::GetInstance()->VIEWPORT_X = Position.X;
	CameraManager::GetInstance()->VIEWPORT_Y = Position.Y;

	if (DraggingWindow)
	{
		Vector2 NewPosition = Input::GetInstance()->MousePos - DragOffset;
		SetWindowPosition(NewPosition);
	}

	//glm::vec2 MousePosViewport = Input::GetInstance()->MousePos;
	////MousePosViewport += ViewportOffset;
	//glm::vec2 TopLeft = Position;
	//glm::vec2 BottomRight = Position + Size;
	//bool InWindow = (MousePosViewport.x > TopLeft.x && MousePosViewport.x < BottomRight.x && MousePosViewport.y < BottomRight.y && MousePosViewport.y > TopLeft.y);
	//
	//if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_PRESS && InWindow)
	//{
	//	DragOffset = Input::GetInstance()->MousePos - Position;
	//	DraggingWindow = true;
	//	//PopOut();
	//}
	//else if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_HOLD && DraggingWindow)
	//{
	//	glm::vec2 NewPosition = Input::GetInstance()->MousePos - DragOffset;
	//	SetWindowPosition(NewPosition);
	//}
	//else if(Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_RELEASE)
	//{
	//	if (DraggingWindow)
	//	{
	//		glm::vec2 NewPosition = Input::GetInstance()->MousePos - DragOffset;
	//		SetWindowPosition(NewPosition);

	//		DraggingWindow = false;
	//	}
	//	
	//}

	for (std::shared_ptr<UIElement> UIElement : UIElements)
	{
		UIElement->BaseUpdate();
	}

	//Input::GetInstance()->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
}

Vector2 EditorWindow::GetPosition()
{
	if (ParentWindow != EditorWindowManager::MainWindow)
	{
		int x, y;
		glfwGetWindowPos(ParentWindow, &x, &y);
		Position = glm::vec2(x, y);
	}
	return Position;
}

void EditorWindow::SetWindowPosition(Vector2 _position)
{
	if (ParentWindow != EditorWindowManager::MainWindow)
	{
		glfwSetWindowPos(ParentWindow, _position.X, _position.Y);
	}
	Position = _position;
}

void EditorWindow::MainWindowSizeChanged(int _w, int _h)
{
	if (ParentWindow != EditorWindowManager::MainWindow) //seperate window
	{
		Size = glm::vec2(_w, _h);
	}
	//glutSetWindow(WindowID);

	//CameraManager::GetInstance()->SCR_HEIGHT = _h;
	//CameraManager::GetInstance()->SCR_WIDTH = _w;

	// resize and reposition the sub window
	//glutPositionWindow(border, (h + border) / 2);
	//glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
}
