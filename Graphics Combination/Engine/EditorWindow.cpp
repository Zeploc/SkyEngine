#include "EditorWindow.h"


#include "Input.h"
#include "UIButton.h"
#include "EditorWindowManager.h"
#include "Camera.h"

//#include <freeglut.h>
#include <glfw3.h>
#include <iostream>

EditorWindow::EditorWindow(std::string _WindowName, GLFWwindow*  _ParentWindow, glm::vec2 _Size, glm::vec2 _Position)
	: Size(_Size), Position(_Position), WindowName(_WindowName), ParentWindow(_ParentWindow)
{
	if (!_ParentWindow)
	{
		// make new window
		//glutInitWindowPosition(Position.x, Position.y);
		//glutInitWindowSize(Size.x, Size.y);
		//WindowID = glutCreateWindow(WindowName.c_str());
	}
	else
	{

	}

	
	//SetupGlutBindings();
	
	EditorWindowManager::NewWindowCreated(this);
	
	SetupUI();

	//Camera::GetInstance()->Init(Size.x, Size.y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
	//Camera::GetInstance()->SwitchProjection(Camera::PERSPECTIVE);
	
	//glutSetWindow(CurrentWindow);

}

void EditorWindow::SetupGlutBindings()
{

	//glutDisplayFunc(*EditorWindowManager::StaticRenderWindow);

}

void EditorWindow::SetupUI()
{
	std::shared_ptr<UIImage> Back(new UIImage(glm::vec2(Size.x / 2.0f, Size.y / 2.0f), Utils::CENTER, 0.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), Size.x, Size.y));
	//TestBtn->AddText("Test", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	UIElements.push_back(Back);


	//20, Size.y - 20.0f
	std::shared_ptr<UIButton> TestBtn(new UIButton(glm::vec2(Size.x / 2.0f, 20.0f), Utils::TOP_CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 260, 50, nullptr));

	//FDelegate<EditorWindow>* cbk = new FDelegate<EditorWindow>(*this, &EditorWindow::TestButtonPressed);
	//(*cbk)();
	TestBtn->BindPress(FDelegate<EditorWindow>(this, &EditorWindow::TestButtonPressed));// cbk);// std::bind(&EditorWindow::TestButtonPressed, this));
	//TestBtn->AddText("Test", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	UIElements.push_back(TestBtn);

}

EditorWindow::~EditorWindow()
{
}


void EditorWindow::RenderWindow()
{
	//glClearColor(ClearColour.r, ClearColour.g, ClearColour.b, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	int width, height;
	glfwGetWindowSize(ParentWindow, &width, &height);
	

	glViewport(Position.x, height - Position.y - Size.y, Size.x, Size.y);
	Camera::GetInstance()->SCR_WIDTH = Size.x;
	Camera::GetInstance()->SCR_HEIGHT = Size.y;
	Camera::GetInstance()->VIEWPORT_X = Position.x;
	Camera::GetInstance()->VIEWPORT_Y = Position.y;

	glDisable(GL_DEPTH_TEST);

	
	for (std::shared_ptr<UIElement> UIElement : UIElements)
	{
		UIElement->DrawUIElement();
	}


	//glutSwapBuffers();
}

void EditorWindow::PopOut()
{
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
	//Camera::GetInstance()->Init(Size.x, Size.y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
	//Camera::GetInstance()->SwitchProjection(Camera::PERSPECTIVE);
}

bool EditorWindow::IsPointInWindow(glm::vec2 _point)
{
	if (_point.x < Position.x || _point.y < Position.y)
		return false;

	glm::vec2 EndPos = Position + Size;

	if (_point.x > EndPos.x || _point.y > EndPos.y)
		return false;
	return true;
}

void EditorWindow::TestButtonPressed()
{
	PopOut();
}

void EditorWindow::UpdateWindow()
{
	Camera::GetInstance()->SCR_WIDTH = Size.x;
	Camera::GetInstance()->SCR_HEIGHT = Size.y;
	Camera::GetInstance()->VIEWPORT_X = Position.x;
	Camera::GetInstance()->VIEWPORT_Y = Position.y;

	glm::vec2 MousePosViewport = Input::GetInstance()->MousePos;
	//MousePosViewport += ViewportOffset;
	glm::vec2 TopLeft = Position;
	glm::vec2 BottomRight = Position + Size;
	bool InWindow = (MousePosViewport.x > TopLeft.x && MousePosViewport.x < BottomRight.x && MousePosViewport.y < BottomRight.y && MousePosViewport.y > TopLeft.y);
	
	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_PRESS && InWindow)
	{
		DragOffset = Input::GetInstance()->MousePos - Position;
		DraggingWindow = true;
		//PopOut();
	}
	else if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_HOLD && DraggingWindow)
	{
		glm::vec2 NewPosition = Input::GetInstance()->MousePos - DragOffset;
		SetWindowPosition(NewPosition);
	}
	else if(Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_RELEASE)
	{
		if (DraggingWindow)
		{
			glm::vec2 NewPosition = Input::GetInstance()->MousePos - DragOffset;
			SetWindowPosition(NewPosition);

			DraggingWindow = false;
		}
		
	}
	
	for (std::shared_ptr<UIElement> UIElement : UIElements)
	{
		UIElement->BaseUpdate();
	}

	//Input::GetInstance()->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
}

void EditorWindow::SetWindowPosition(glm::vec2 _position)
{
	Position = _position;
	//glutSetWindow(WindowID);
	//glutPositionWindow(Position.x, Position.y);
}

void EditorWindow::MainWindowSizeChanged(int _w, int _h)
{
	//glutSetWindow(WindowID);

	//Camera::GetInstance()->SCR_HEIGHT = _h;
	//Camera::GetInstance()->SCR_WIDTH = _w;

	// resize and reposition the sub window
	//glutPositionWindow(border, (h + border) / 2);
	//glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
}
