//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    main.cpp
// Description		:    main implementation of the program
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <iostream>
#include <time.h>
#include <windows.h>
#include <Box2D.h>
//#include <vld.h>

// OpenGL Library Includes //
#include <glew.h>
#include <GLFW/glfw3.h>

// Engine Includes //
#include "Editor/Scene/EditorScene.h"
#include "Engine/Input/Input.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Sound/SoundManager.h"
#include "Engine/System/Time.h"
#include "Engine/System/LogManager.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Engine/Camera/Camera.h"
#include "Editor/Windows/EditorWindow.h"
#include "Engine/Render/FrameBuffer.h"
#include "Engine/Render/Shader.h"
#include "Game/Scenes/Level.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

// Static Function Prototypes //

// Types //
using namespace std;

#define SI Input::GetInstance()
#define CAM Camera::GetInstance() 
#define SM SceneManager::GetInstance()

// GLUT //
void renderScene(void);
void changeSize(int w, int h);
void Update();
void SetupGLUT(int &argc, char ** argv);
void Init();
void OnExit();

void glfw_onError(int error, const char * description);

// GLFW
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

bool bLoading = true;

float CurrentTimer = 0.0f;
float TickRate = 60.0f;
double dPrevTime = 0.0f;


glm::vec2 MainWindowSize = glm::vec2(1280, 720);
glm::vec3 SkyColour = glm::vec3(0.3f, 0.8f, 0.9f);

/************************************************************
#--Description--#: 	The main function of the program
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Count of how many arguments and a dynamic array of each argument
#--Return--#: 		Return int
************************************************************/
int main(int argc, char **argv)
{
	srand(unsigned int(time(NULL)));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	Lighting::m_v4FogColour = glm::vec4(SkyColour, 1.0f);

	GLFWwindow* window = glfwCreateWindow(MainWindowSize.x, MainWindowSize.y, "Editor", NULL, NULL);
	EditorWindowManager::SetMainWindow(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	CAM->Init(MainWindowSize.x, MainWindowSize.y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
	// CAM->SwitchProjection(EProjectionMode::Perspective);
	CAM->MainWindow = window;

	glViewport(0, 0, MainWindowSize.x, MainWindowSize.y);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetErrorCallback(glfw_onError);
	
	glfwSetWindowFocusCallback(window, EditorWindowManager::FocusChanged);

	// OpenGL init
	glewInit();

	// Settings Initialised
	Init();

	// The input function registration
	SI->Init(window);

	//SetupGLUT(argc, argv);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(SkyColour.r, SkyColour.g, SkyColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, MainWindowSize.x, MainWindowSize.y);
		CAM->SCR_WIDTH = MainWindowSize.x;
		CAM->SCR_HEIGHT = MainWindowSize.y;
		CAM->VIEWPORT_X = 0;
		CAM->VIEWPORT_Y = 0;

		Update();
		CAM->SCR_WIDTH = MainWindowSize.x;
		CAM->SCR_HEIGHT = MainWindowSize.y;
		CAM->VIEWPORT_X = 0;
		CAM->VIEWPORT_Y = 0;

		renderScene();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	OnExit();

	return 0;
}

void glfw_onError(int error, const char* description)
{
	// print message in Windows popup dialog box
	MessageBox(NULL, description, "GLFW error", MB_OK);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/************************************************************
#--Description--#: 	Update for every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Update()
{
	//glutSetWindow(GameWindowID);
	if (bLoading)
	{
		SoundManager::GetInstance()->InitFMod();
		// Scene setup here
		std::shared_ptr<Level> LevelScene = std::shared_ptr<Level>(new Level("Demo Environment"));		
		// std::shared_ptr<EditorScene> LevelScene = std::shared_ptr<EditorScene>(new EditorScene("Editor"));
		
		SceneManager::GetInstance()->AddScene(LevelScene);
		bLoading = false;
	}
	else
	{
		double dCurrentTime = glfwGetTime();//glutGet(GLUT_ELAPSED_TIME);
		double TimeDelta = (dCurrentTime - dPrevTime);// / 1000;
		dPrevTime = glfwGetTime();//glutGet(GLUT_ELAPSED_TIME);
		CurrentTimer += TimeDelta;
		if (CurrentTimer > 1.0f / TickRate)
		{
			Camera::GetInstance()->Update();
			SceneManager::GetInstance()->UpdateCurrentScene();
			Time::Update();
			SI->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
			CurrentTimer = 0.0f;
		}

	}

	//glutPostRedisplay();

}

/************************************************************
#--Description--#: 	Renders the elements every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void renderScene()
{
	glfwMakeContextCurrent(EditorWindowManager::MainWindow);
	if (bLoading)
	{
		glClearColor(0.8f, 0.8f, 0.8f, 1.0); // clear grey
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LogManager::GetInstance()->Render();
	}
	else
	{
		SM->RenderCurrentScene();
	}
}

/************************************************************
#--Description--#: 	Updated when the window changes size
#--Author--#: 		Alex Coultas
#--Parameters--#: 	The width and height of the window
#--Return--#: 		NA
************************************************************/
void changeSize(int w, int h)
{
	//glutSetWindow(GameWindowID);
	CAM->SCR_HEIGHT = h;
	CAM->SCR_WIDTH = w;

	EditorWindowManager::MainWindowSizeChanged(w, h);
}



/************************************************************
#--Description--#: 	Initialises the program
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Init()
{
	Shader::LoadAllDefaultShadersInCurrentContext();
	////Shader::AddProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/FragmentShader.fs", "BaseProgram");
	//Shader::AddProgram("Resources/Shaders/TextureVertexShader.vs", "Resources/Shaders/TextureFragmentShader.fs", "BaseProgram");
	//Shader::AddComputeProgram("Resources/Shaders/ComputeShader.comps", "ComputeProgram");
	//Shader::AddProgram("Resources/Shaders/ParticleShaderCompute.vs", "Resources/Shaders/ParticleShaderCompute.fs", "GPUParticlesProgram");
	//Shader::AddProgram("Resources/Shaders/ParticleShader.vs", "Resources/Shaders/ParticleShader.fs", "ParticleShader", "Resources/Shaders/ParticleShader.gs");
	//Shader::AddProgram("Resources/Shaders/LitVertexShader.vs", "Resources/Shaders/LitFragmentShader.fs", "LitTextureprogram");
	//Shader::AddProgram("Resources/Shaders/AnimatedModel.vs", "Resources/Shaders/AnimatedModel.fs", "AnimatedModel");
	//Shader::AddProgram("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs", "TextUIprogram");
	//Shader::AddProgram("Resources/Shaders/UI.vs", "Resources/Shaders/UI.fs", "UIprogram");
	//Shader::AddProgram("Resources/Shaders/CubeMapVertexShader.vs", "Resources/Shaders/CubeMapFragmentShader.fs", "CubeMapProgram");
	//Shader::AddProgram("Resources/Shaders/ModelVertexShader.vs", "Resources/Shaders/ModelFragmentShader.fs", "ModelProgram");
	//Shader::AddProgram("Resources/Shaders/ModelVertexShaderLit.vs", "Resources/Shaders/ModelFragmentShaderLit.fs",  "ModelProgramLit");
	//Shader::AddProgram("Resources/Shaders/ReflectionShader.vs", "Resources/Shaders/ReflectionShader.fs", "ReflectionProgram");
	//Shader::AddProgram("Resources/Shaders/FogShader.vs", "Resources/Shaders/FogShader.fs", "FogProgram");
	//Shader::AddProgram("Resources/Shaders/GeometryShader.vs", "Resources/Shaders/GeometryShader.fs", "GeometryShader", "Resources/Shaders/GeometryShader.gs");
	//Shader::AddProgram("Resources/Shaders/ParticleShader.vs", "Resources/Shaders/ParticleShader.fs", "ParticleShader", "Resources/Shaders/ParticleShader.gs");
	//Shader::AddTessProgram("Resources/Shaders/Tessellation.vs", "Resources/Shaders/Tessellation.fs", "Resources/Shaders/TessControl.tc", "Resources/Shaders/TessEval.te", "TessProgram");
	//Shader::AddProgram("Resources/Shaders/FrameBuffer.vs", "Resources/Shaders/FrameBuffer.fs", "FrameBuffer");

	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glutSetCursor(GLUT_CURSOR_NONE);

	//glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	//glutIgnoreKeyRepeat(1);

	LogManager::GetInstance()->Init();

}

/************************************************************
#--Description--#: 	Runs when the application closes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void OnExit()
{
	Shader::CleanUp();
	SceneManager::DestoryInstance();
	Camera::DestoryInstance();
	//Input::DestoryInstance();
	SoundManager::DestoryInstance();
	Text::Fonts.~vector();
}



