#include "EditorWindow.h"

#include <freeglut.h>
#include <iostream>

#include "Input.h"

#include "EditorWindowManager.h"

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
	

	glutDisplayFunc(*EditorWindowManager::StaticRenderWindow);

	WindowInput = new Input;
	WindowInput->Init();
	
	EditorWindowManager::NewWindowCreated(this);
	
	
	glutSetWindow(CurrentWindow);

}

EditorWindow::~EditorWindow()
{
}


void EditorWindow::RenderWindow()
{
	glutSetWindow(WindowID);
	glClearColor(ClearColour.r, ClearColour.g, ClearColour.b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
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
	if (WindowInput->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_PRESS)
	{
		DraggingWindow = true;
		DragOffset = WindowInput->MousePos;
	}
	else if (WindowInput->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_RELEASE)
	{
		DraggingWindow = false;
	}
	else
	{
		if (DraggingWindow)
		{
			glm::vec2 NewPosition = Position + WindowInput->MousePos - DragOffset;

			//std::cout << "Mouse Pos x:" << Input::GetInstance()->MousePos.x << " y: " << Input::GetInstance()->MousePos .y << std::endl;
			SetWindowPosition(NewPosition);
		}
	}

	glutSetWindow(WindowID);
	glutPostRedisplay();

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
