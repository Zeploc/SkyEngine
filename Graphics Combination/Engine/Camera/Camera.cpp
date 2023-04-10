// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Camera.h"

// Library Includes //
#include <algorithm>    // std::max

// OpenGL Library Includes //
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Engine Includes //
#include "Engine/Input/Input.h"
#include "Engine/Math/Matrix.h"
#include "Engine/Math/Vector.h"

// Static Variables //
std::map<int, Camera*> Camera::m_pCameras;
Camera* Camera::m_pCamera;

/************************************************************
#--Description--#: 	Initialises the camera to the screen size and camera vectors
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in screen size and camera vectors
#--Return--#: 		NA
************************************************************/
void Camera::Init(int ScreenWidth, int ScreenWidthheight, glm::vec3 CamPos, glm::vec3 ForwardVec, glm::vec3 UpVec)
{
	SCR_WIDTH = ScreenWidth;
	SCR_HEIGHT = ScreenWidthheight;
	CameraPosition = CamPos;
	CameraForward = ForwardVec;
	CameraUp = UpVec;

	SwitchProjection(ProjectionMode);
	
	//projection = glm::perspective(90.0f, (float)SCR_WIDTH /	(float)SCR_HEIGHT, 0.1f, 100.0f);

	// float HalfWidth = static_cast<float>(SCR_WIDTH) / fWindowScale;
	// float HalfHeight = static_cast<float>(SCR_HEIGHT) / fWindowScale;
	// projection = glm::ortho(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 0.1f, 100.0f);
	// View.SetViewTranslation(CameraPosition);
	// View.SetCameraOrientation(CameraPosition + CameraForward, CameraUp);
	
	View.SetLookAt(CameraPosition,
				  CameraPosition + CameraForward,
				  CameraUp);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Camera::Update()
{
	/*if (ProjectionMode == Perspective && m_bFPS)
		FPSControls();*/
	if (bUseSpectatorControls)
	{
		SpectatorControls();
	}
	
	// view = lookAt(CameraPosition,
	//               CameraPosition + CameraForward,
	//               CameraUp);
	//View = Matrix4(view);
	View.SetLookAt(CameraPosition,
				  CameraPosition + CameraForward,
				  CameraUp);
	
	// TODO: Complete
	//View.SetViewTranslation(CameraPosition);
	//View.SetCameraOrientation(CameraForward, CameraUp);
}

Vector3 Camera::GetCameraRightVector()
{
	return CameraUp.Cross(CameraForward);
}

/************************************************************
#--Description--#: 	Moves the camera
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Camera::MoveCamera(glm::vec3 _Movement)
{
	CameraPosition += _Movement;
	//SetMVP(FTransform());
}

void Camera::EnableSpectatorControls(bool _bSpectatorControls)
{
	bUseSpectatorControls = _bSpectatorControls;
	glfwSetCursorPos(MainWindow, static_cast<double>(SCR_WIDTH) * 0.5, static_cast<double>(SCR_HEIGHT) * 0.5);
}

void Camera::SpectatorControls()
{
	glm::vec2 Offset = glm::vec2(Input::GetInstance()->MousePos.ToGLM() - glm::vec2(static_cast<float>(SCR_WIDTH) * 0.5f, static_cast<float>(SCR_HEIGHT) * 0.5f));
	Offset *= MouseSensitivity;
	Yaw -= Offset.x;
	Pitch -= Offset.y;

	glm::clamp(Pitch, 89.0f, -89.0f);
	glm::vec3 frontVector(-cos(glm::radians(Pitch)) * sin(glm::radians(Yaw)),
	                      sin(glm::radians(Pitch)),
	                      -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)));
	CameraForward = normalize(frontVector);

	if (bSpectatorMovement)
	{
		if (Input::GetInstance()->KeyState[GLFW_KEY_W] == Input::INPUT_HOLD)
		{
			CameraPosition += CameraForward * cameraSpeed * 0.025f;
		}
		else if (Input::GetInstance()->KeyState[GLFW_KEY_S] == Input::INPUT_HOLD)
		{
			CameraPosition -= CameraForward * cameraSpeed * 0.025f;
		}

		if (Input::GetInstance()->KeyState[GLFW_KEY_A] == Input::INPUT_HOLD)
		{
			CameraPosition -= CameraForward.Cross(CameraUp).GetNormalized() * cameraSpeed * 0.025f;
		}
		else if (Input::GetInstance()->KeyState[GLFW_KEY_D] == Input::INPUT_HOLD)
		{
			CameraPosition += CameraForward.Cross(CameraUp).GetNormalized() * cameraSpeed * 0.025f;
		}

		if (Input::GetInstance()->KeyState[GLFW_KEY_E] == Input::INPUT_HOLD)
		{
			CameraPosition += CameraUp * cameraSpeed * 0.025f;
		}
		else if (Input::GetInstance()->KeyState[GLFW_KEY_Q] == Input::INPUT_HOLD)
		{
			CameraPosition -= CameraUp * cameraSpeed * 0.025f;
		}
	}
	glfwSetCursorPos(MainWindow, static_cast<double>(SCR_WIDTH) * 0.5, static_cast<double>(SCR_HEIGHT) * 0.5);
}

/************************************************************
#--Description--#: 	Sets the window projection scale
#--Author--#: 		Alex Coultas
#--Parameters--#: 	New flaot scale
#--Return--#: 		NA
************************************************************/
void Camera::SetWindowScale(float _fNewScale)
{
	fWindowScale = _fNewScale;
	
	SwitchProjection(EProjectionMode::Perspective);
	
	// float HalfWidth = static_cast<float>(SCR_WIDTH) / fWindowScale;
	// float HalfHeight = static_cast<float>(SCR_HEIGHT) / fWindowScale;
	// projection = glm::ortho(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 0.1f, fMaxViewClipping);
}

Vector3 Camera::ScreenToWorldDirection(Vector2 InScreenPosition)
{
	float x = (2.0f * InScreenPosition.X) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * InScreenPosition.Y) / SCR_HEIGHT;
	const Vector2 RayNds = {x, y};

	const Vector4 RayClip = Vector4(RayNds.X, RayNds.Y, -1.0f, 1.0f);
	
	Vector4 RayEye = Projection.GetInverse() * RayClip;
	RayEye = Vector4(RayEye.X, RayEye.Y, -1.0f, 0.0f);

	Vector3 RayWorld = (View.GetInverse() * RayEye).To3F();

	RayWorld = RayWorld.Normalize();
	return RayWorld;
}

Vector3 Camera::ScreenToWorldPosition2D(Vector2 InScreenPosition)
{
	Vector3 PlaneNormal = -CameraForward;
	Vector3 MouseDirection = ScreenToWorldDirection(InScreenPosition);
	float t = -(CameraPosition.Dot(PlaneNormal) / MouseDirection.Dot(PlaneNormal));
	t /= abs(CameraPosition.Z);
	return (MouseDirection * t).ToGLM() + CameraPosition;
}

/************************************************************
#--Description--#: 	Passes in the new mvp to the current program shader
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the transforms vectors
#--Return--#: 		NA
************************************************************/
void Camera::SetMVP(FTransform _transform, GLuint program)
{
	glm::mat4 translate = glm::translate(glm::mat4(), _transform.Position.ToGLM());
	glm::mat4 scale = glm::scale(glm::mat4(), _transform.Scale.ToGLM());
	glm::mat4 rotation = rotate(glm::mat4(), glm::radians(_transform.Rotation.X), glm::vec3(1, 0, 0));
	rotation = rotate(rotation, glm::radians(_transform.Rotation.Y), glm::vec3(0, 1, 0));
	rotation = rotate(rotation, glm::radians(_transform.Rotation.Z), glm::vec3(0, 0, 1));

	glm::mat4 model = translate * rotation * scale;
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(View.ToGLM()));//view));//
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, glm::value_ptr(Projection.ToGLM()));

	glm::mat4 MVP = Projection.ToGLM() * View.ToGLM() * model;
	GLint MVPLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, value_ptr(MVP));
}

/************************************************************
#--Description--#:  Change projection type
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes int mode
#--Return--#: 		NA
************************************************************/
void Camera::SwitchProjection(EProjectionMode InMode)
{
	ProjectionMode = InMode;
	switch (ProjectionMode)
	{
		case EProjectionMode::Orthographic:
			{
				const float HalfWidth = static_cast<float>(SCR_WIDTH) / fWindowScale;
				const float HalfHeight = static_cast<float>(SCR_HEIGHT) / fWindowScale;
				//projection = glm::ortho(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 0.1f, fMaxViewClipping);
				OrthoProjInfo ProjectionInfo;
				ProjectionInfo.Left = -HalfWidth;
				ProjectionInfo.Right = HalfWidth;
				ProjectionInfo.Bottom = -HalfHeight;
				ProjectionInfo.Top = HalfHeight;
				ProjectionInfo.ZNear = 0.1f;
				ProjectionInfo.ZFar = fMaxViewClipping;
				Projection.SetOrthographicProjection(ProjectionInfo);
				break;
			}
		case EProjectionMode::Perspective:
			{
				PersProjInfo ProjectionInfo;
				ProjectionInfo.FOV = 45.0f;
				ProjectionInfo.Width = static_cast<float>(SCR_WIDTH);
				ProjectionInfo.Height = static_cast<float>(SCR_HEIGHT);
				ProjectionInfo.zNear = 0.1f;
				ProjectionInfo.zFar = fMaxViewClipping;
				Projection.SetPerspectiveProjection(ProjectionInfo);
				// projection = glm::perspective(45.0f, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, fMaxViewClipping);
				break;
			}
		default:
			break;
	}
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Camera::Camera()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Camera::~Camera()
{
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
Camera* Camera::GetInstance()
{
	//int currentWindow = glutGetWindow();
	//Camera* CurrentFound = nullptr;
	//auto it = m_pCameras.find(currentWindow);
	//if (it == m_pCameras.end())// null or doesn't exist
	//{
	//	Camera* NewInput = new Camera;
	//	m_pCameras.insert(std::pair<int, Camera*>(currentWindow, NewInput));
	//	CurrentFound = NewInput;
	//}
	//else
	//	CurrentFound = (*it).second;

	//return CurrentFound;

	if (!m_pCamera) // null or doesn't exist
	{
		m_pCamera = new Camera;
	}
	return m_pCamera;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Camera::DestoryInstance()
{
	//int currentWindow = glutGetWindow();
	//auto it = m_pCameras.find(currentWindow);
	//if (it._Ptr)// exists
	//{
	//	delete (*it).second;
	//	m_pCameras.erase(currentWindow);
	//}

	if (m_pCamera)
	{
		delete m_pCamera;
	}
	m_pCamera = nullptr;
}
