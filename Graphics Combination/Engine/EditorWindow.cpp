#include "EditorWindow.h"


#include "Input.h"
#include "UIButton.h"
#include "EditorWindowManager.h"
#include "Camera.h"

#include <freeglut.h>
#include <iostream>

EditorWindow::EditorWindow(std::string _WindowName, int _ParentWindow, glm::vec2 _Size, glm::vec2 _Position)
	: Size(_Size), Position(_Position), WindowName(_WindowName)
{
	int CurrentWindow = glutGetWindow();

	if (_ParentWindow == -1)
	{
		glutInitWindowPosition(Position.x, Position.y);
		glutInitWindowSize(Size.x, Size.y);
		WindowID = glutCreateWindow(WindowName.c_str());
	}
	else
	{
		WindowID = glutCreateSubWindow(_ParentWindow, Position.x, Position.y, Size.x, Size.y);
	}
	
	SetupGlutBindings();
	
	EditorWindowManager::NewWindowCreated(this);
	
	SetupUI();
	
	glutSetWindow(CurrentWindow);

}

void EditorWindow::SetupGlutBindings()
{

	//glutDisplayFunc(boost::bind(&EditorWindow::RenderWindow, &this));
	glutDisplayFunc(*EditorWindowManager::StaticRenderWindow);

	WindowInput = new Input;
	WindowInput->Init();
}

void EditorWindow::SetupUI()
{
	std::shared_ptr<UIButton> TestBtn(new UIButton(glm::vec2(20, Size.y - 20.0f), Utils::BOTTOM_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 260, 50, nullptr));
	//TestBtn->AddText("QUIT", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	UIElements.push_back(TestBtn);
}

EditorWindow::~EditorWindow()
{
}


void EditorWindow::RenderWindow()
{
	Camera::GetInstance()->SCR_HEIGHT = Size.y;
	Camera::GetInstance()->SCR_WIDTH = Size.x;

	glutSetWindow(WindowID);
	glClearColor(ClearColour.r, ClearColour.g, ClearColour.b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	
	for (std::shared_ptr<UIElement> UIElement : UIElements)
	{
		UIElement->DrawUIElement();
	}


	glutSwapBuffers();
}

void EditorWindow::PopOut()
{
	glutSetWindow(EditorWindowManager::iMainWindowID);
	int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
	int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);

	glutDestroyWindow(WindowID);
	EditorWindowManager::WindowRemoved(WindowID);

	glm::vec2 NewWindowPos = Position;
	NewWindowPos.x += screen_pos_x;
	NewWindowPos.y += screen_pos_y;

	glutInitWindowPosition(NewWindowPos.x, NewWindowPos.y);
	glutInitWindowSize(Size.x, Size.y);
	WindowID = glutCreateWindow(WindowName.c_str());
	SetupGlutBindings();
	EditorWindowManager::NewWindowCreated(this);
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

void EditorWindow::UpdateWindow()
{
	Input::SetInstance(WindowInput);

	Camera::GetInstance()->SCR_HEIGHT = Size.y;
	Camera::GetInstance()->SCR_WIDTH = Size.x;


	if (WindowInput->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_PRESS)
	{
		DragOffset = WindowInput->MousePos;
		PrevMouse = WindowInput->MousePos;

		//PopOut();
	}
	else if (WindowInput->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_RELEASE)
	{
		glm::vec2 NewPosition = Position + WindowInput->MousePos - DragOffset;

		//std::cout << "Mouse Pos x:" << WindowInput->MousePos.x << " y: " << WindowInput->MousePos.y << std::endl;
		SetWindowPosition(NewPosition);
	}
	else if(WindowInput->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_HOLD)
	{
		/*glutSetWindow(WindowID);
		int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
		int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);


		glm::vec2 CurrentWindowPos;
		CurrentWindowPos.x = screen_pos_x;
		CurrentWindowPos.y = screen_pos_y;*/

		glm::vec2 NewPosition = Position + WindowInput->MousePos - DragOffset;

		//if (PrevMouse != WindowInput->MousePos)
		//	std::cout << "Mouse Pos x:" << WindowInput->MousePos.x << " y: " << WindowInput->MousePos.y << std::endl;
		//SetWindowPosition(NewPosition);
		
		PrevMouse = WindowInput->MousePos;
	}

	glutSetWindow(WindowID);
	glutPostRedisplay();

	for (std::shared_ptr<UIElement> UIElement : UIElements)
	{
		UIElement->BaseUpdate();
	}

	WindowInput->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
}

void EditorWindow::SetWindowPosition(glm::vec2 _position)
{
	Position = _position;
	glutSetWindow(WindowID);
	glutPositionWindow(Position.x, Position.y);
}

void EditorWindow::MainWindowSizeChanged(int _w, int _h)
{
	glutSetWindow(WindowID);


	// resize and reposition the sub window
	//glutPositionWindow(border, (h + border) / 2);
	//glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
}
