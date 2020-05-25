#include "EditorWindow.h"

#include <freeglut.h>
#include <iostream>

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
		WindowID = glutCreateSubWindow(_ParentWindow, 10, 10, 300, 300);
	}
	

	glutDisplayFunc(*EditorWindowManager::StaticRenderWindow);
	
	EditorWindowManager::NewWindowCreated(this);

	glutKeyboardFunc(EditorTestProcessNormalKeysDown);


	glutSetWindow(CurrentWindow);


}

void EditorTestProcessNormalKeysDown(unsigned char key, int x, int y)
{
	std::cout << "Key Press " << std::endl;
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

void EditorWindow::UpdateWindow()
{
	glutSetWindow(WindowID);
	glutPostRedisplay();
}

void EditorWindow::MainWindowSizeChanged(int _w, int _h)
{
	glutSetWindow(WindowID);
	// resize and reposition the sub window
	//glutPositionWindow(border, (h + border) / 2);
	//glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
}
