// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "CameraManager.h"

// Library Includes //
#include <algorithm>    // std::max

// OpenGL Library Includes //
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Engine Includes //
#include "Core/EngineWindow.h"
#include "Input/Input.h"
#include "Math/FTransform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Platform/Window/GraphicsWindow.h"
#include "System/TimeManager.h"

// Static Variables //
std::map<int, CameraManager*> CameraManager::m_pCameras;
CameraManager* CameraManager::m_pCamera;

/************************************************************
#--Description--#: 	Initialises the camera to the screen size and camera vectors
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in screen size and camera vectors
#--Return--#: 		NA
************************************************************/
void CameraManager::Init(int ScreenWidth, int ScreenHeight, Vector3 CamPos, Vector3 ForwardVec, Vector3 UpVec)
{
	SCR_WIDTH = ScreenWidth;
	SCR_HEIGHT = ScreenHeight;
	CameraPosition = CamPos;
	CameraForward = ForwardVec;
	CameraUp = UpVec;

	SwitchProjection(ProjectionMode);
	
	//projection = glm::perspective(90.0f, (float)SCR_WIDTH /	(float)SCR_HEIGHT, 0.1f, 100.0f);

	// float HalfWidth = static_cast<float>(SCR_WIDTH) / fWindowScale;
	// float HalfHeight = static_cast<float>(SCR_HEIGHT) / fWindowScale;
	// projection = glm::ortho(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 0.1f, 100.0f);
	
	UpdateViewMatrix();
}

void CameraManager::UpdateViewMatrix()
{
	// View.SetViewTranslation(CameraPosition);
	// View.SetCameraOrientation(CameraPosition + CameraForward, CameraUp);
	View.SetLookAt(CameraPosition,
	               CameraPosition + CameraForward,
	               CameraUp);
}

Vector3 CameraManager::GetCameraRightVector() const
{
	return CameraForward.Cross(CameraUp).GetNormalized();
}

void CameraManager::SetCameraForwardVector(Vector3 _Forward)
{
	CameraForward = _Forward;
	UpdateViewMatrix();
}

void CameraManager::SetCameraPos(Vector3 NewPos)
{
	CameraPosition = NewPos;
	UpdateViewMatrix();
}

/************************************************************
#--Description--#: 	Moves the camera
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void CameraManager::MoveCamera(Vector3 _Movement)
{
	CameraPosition += _Movement;
	UpdateViewMatrix();
	//SetMVP(FTransform());
}

Vector2 CameraManager::GetScreenCenter() const
{
	return {static_cast<float>(SCR_WIDTH) * 0.5f, static_cast<float>(SCR_HEIGHT) * 0.5f};
}

void CameraManager::EnableSpectatorControls(bool _bSpectatorControls)
{
	bUseSpectatorControls = _bSpectatorControls;
	if (bUseSpectatorControls)
	{
		PreviousMousePosition = Input::GetInstance()->MousePos;
		MainWindow->GetGraphicsWindow()->SetCursorPosition(GetScreenCenter());
	}
	else
	{		
		MainWindow->GetGraphicsWindow()->SetCursorPosition(PreviousMousePosition);
	}
}

void CameraManager::SpectatorUpdate()
{
	if (!bUseSpectatorControls)
	{
		return;
	}

	Vector2 Offset = Input::GetInstance()->MousePos - GetScreenCenter();
	Offset *= MouseSensitivity;
	CameraForward.Rotate(Offset.X, Vector3(0,1,0));
	CameraForward.Rotate(Offset.Y, GetCameraRightVector());
	// Needs Clamp
	// GetCameraForwardVector().Rotate(Offset.Y, Vector3(1,0,0));
	// Rotator CameraRotation = GetCameraForwardVector().ToRotator();
	// //CameraRotation.Yaw -= Offset.X;
	// CameraRotation.Pitch -= Offset.Y;
	//
	// // CameraRotation.Pitch = glm::clamp(CameraRotation.Pitch, -89.0f, 89.0f);
	// const Vector3 ForwardVector = CameraRotation.ToVector();
	// CameraForward = ForwardVector.GetNormalized();	

	if (bSpectatorMovement)
	{
		const Vector3 ForwardMovement = CameraForward * CameraSpeed * TimeManager::GetDeltaTime();
		if (Input::GetInstance()->KeyState[GLFW_KEY_W] == Input::INPUT_HOLD)
		{
			CameraPosition += ForwardMovement;
		}
		else if (Input::GetInstance()->KeyState[GLFW_KEY_S] == Input::INPUT_HOLD)
		{
			CameraPosition -= ForwardMovement;
		}

		const Vector3 RightMovement = GetCameraRightVector() * CameraSpeed * TimeManager::GetDeltaTime();
		if (Input::GetInstance()->KeyState[GLFW_KEY_A] == Input::INPUT_HOLD)
		{
			CameraPosition -= RightMovement;
		}
		else if (Input::GetInstance()->KeyState[GLFW_KEY_D] == Input::INPUT_HOLD)
		{
			CameraPosition += RightMovement;
		}

		const Vector3 UpMovement = CameraUp * CameraSpeed * TimeManager::GetDeltaTime();
		if (Input::GetInstance()->KeyState[GLFW_KEY_E] == Input::INPUT_HOLD)
		{
			CameraPosition += UpMovement;
		}
		else if (Input::GetInstance()->KeyState[GLFW_KEY_Q] == Input::INPUT_HOLD)
		{
			CameraPosition -= UpMovement;
		}
	}
	MainWindow->GetGraphicsWindow()->SetCursorPosition(GetScreenCenter());
}

/************************************************************
#--Description--#: 	Sets the window projection scale
#--Author--#: 		Alex Coultas
#--Parameters--#: 	New flaot scale
#--Return--#: 		NA
************************************************************/
void CameraManager::SetWindowScale(float _fNewScale)
{
	fWindowScale = _fNewScale;
	
	SwitchProjection(EProjectionMode::Perspective);
	
	// float HalfWidth = static_cast<float>(SCR_WIDTH) / fWindowScale;
	// float HalfHeight = static_cast<float>(SCR_HEIGHT) / fWindowScale;
	// projection = glm::ortho(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 0.1f, fMaxViewClipping);
}

Vector3 CameraManager::ScreenToWorldDirection(Vector2 InScreenPosition)
{
	float x = (2.0f * InScreenPosition.X) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * InScreenPosition.Y) / SCR_HEIGHT;
	const Vector2 RayNds = {x, y};

	const Vector4 RayClip = Vector4(RayNds.X, RayNds.Y, -1.0f, 1.0f);
	
	Vector4 RayEye = Projection.GetInverse() * RayClip;
	RayEye = Vector4(RayEye.X, RayEye.Y, -1.0f, 0.0f);

	Vector3 RayWorld = Vector3(View.GetInverse().ToGLM() * RayEye);

	RayWorld = RayWorld.Normalize();
	return RayWorld;
}

Vector3 CameraManager::ScreenToWorldPosition2D(Vector2 InScreenPosition)
{
	Vector3 PlaneNormal = -CameraForward;
	Vector3 MouseDirection = ScreenToWorldDirection(InScreenPosition);
	float t = -(CameraPosition.Dot(PlaneNormal) / MouseDirection.Dot(PlaneNormal));
	t /= abs(CameraPosition.Z);
	return (MouseDirection * t) + CameraPosition;
}

glm::mat4 CameraManager::GetMVP(glm::mat4 model) const
{
	return Projection.ToGLM() * View.ToGLM() * model;
}

/************************************************************
#--Description--#: 	Passes in the new mvp to the current program shader
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the transforms vectors
#--Return--#: 		NA
************************************************************/
void CameraManager::SetMVP(FTransform InTransform, GLuint program)
{
	glm::mat4 model = InTransform.GetModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(View.ToGLM()));//view));//
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, glm::value_ptr(Projection.ToGLM()));

	glm::mat4 MVP = GetMVP(model);
	GLint MVPLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, value_ptr(MVP));
}

/************************************************************
#--Description--#:  Change projection type
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes int mode
#--Return--#: 		NA
************************************************************/
void CameraManager::SwitchProjection(EProjectionMode InMode)
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
CameraManager::CameraManager()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
CameraManager::~CameraManager()
{
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
CameraManager* CameraManager::GetInstance()
{
	//int currentWindow = glutGetWindow();
	//CameraManager* CurrentFound = nullptr;
	//auto it = m_pCameras.find(currentWindow);
	//if (it == m_pCameras.end())// null or doesn't exist
	//{
	//	CameraManager* NewInput = new CameraManager;
	//	m_pCameras.insert(std::pair<int, CameraManager*>(currentWindow, NewInput));
	//	CurrentFound = NewInput;
	//}
	//else
	//	CurrentFound = (*it).second;

	//return CurrentFound;

	if (!m_pCamera) // null or doesn't exist
	{
		m_pCamera = new CameraManager;
	}
	return m_pCamera;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void CameraManager::DestoryInstance()
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
