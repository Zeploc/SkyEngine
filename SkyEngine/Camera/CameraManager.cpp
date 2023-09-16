// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "CameraManager.h"

// Library Includes //
#include <algorithm>    // std::max

// OpenGL Library Includes //
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Engine Includes //
#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"
#include "Input/Input.h"
#include "Canvas/ViewportLayer.h"
#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "System/LogManager.h"
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
void CameraManager::Init(CViewportLayer* InViewportLayer, SVector CamPos, SVector ForwardVec, SVector UpVec)
{
	Viewport = InViewportLayer;
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

SVector CameraManager::GetCameraRightVector() const
{
	return CameraForward.Cross(CameraUp).GetNormalized();
}

void CameraManager::SetCameraForwardVector(SVector _Forward)
{
	CameraForward = _Forward;
	UpdateViewMatrix();
}

void CameraManager::SetCameraPos(SVector NewPos)
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
void CameraManager::MoveCamera(SVector _Movement)
{
	CameraPosition += _Movement;
	UpdateViewMatrix();
	//SetMVP(FTransform());
}

SVector2i CameraManager::GetScreenCenter() const
{
	return Viewport->GetViewportSize() / 2;
}

void CameraManager::EnableSpectatorControls(bool _bSpectatorControls)
{
	bUseSpectatorControls = _bSpectatorControls;
	if (bUseSpectatorControls)
	{
		PreviousMousePosition = GetApplication()->GetApplicationWindow()->GetInput().MousePos;
		Viewport->GetOwningWindow().lock()->SetCursorPosition(GetScreenCenter());
	}
	else
	{		
		Viewport->GetOwningWindow().lock()->SetCursorPosition(PreviousMousePosition);
	}
}

void CameraManager::SpectatorUpdate()
{
	if (!bUseSpectatorControls)
	{
		return;
	}

	const SVector2i ScreenOffset = GetApplication()->GetApplicationWindow()->GetInput().MousePos - GetScreenCenter();
	const SVector2 Offset = SVector2(ScreenOffset) * MouseSensitivity;
	CameraForward.Rotate(Offset.X, SVector(0,1,0));
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
		const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
		const SVector ForwardMovement = CameraForward * CameraSpeed * CTimeManager::GetDeltaTime();
		if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_W] == CInput::INPUT_HOLD)
		{
			CameraPosition += ForwardMovement;
		}
		else if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_S] == CInput::INPUT_HOLD)
		{
			CameraPosition -= ForwardMovement;
		}
		
		const SVector RightMovement = GetCameraRightVector() * CameraSpeed * CTimeManager::GetDeltaTime();
		if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_A] == CInput::INPUT_HOLD)
		{
			CameraPosition -= RightMovement;
		}
		else if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_D] == CInput::INPUT_HOLD)
		{
			CameraPosition += RightMovement;
		}
		
		const SVector UpMovement = CameraUp * CameraSpeed * CTimeManager::GetDeltaTime();
		if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_E] == CInput::INPUT_HOLD)
		{
			CameraPosition += UpMovement;
		}
		else if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_Q] == CInput::INPUT_HOLD)
		{
			CameraPosition -= UpMovement;
		}
	}
	Viewport->GetOwningWindow().lock()->SetCursorPosition(GetScreenCenter());
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

SVector CameraManager::ScreenToWorldDirection(SVector2i InScreenPosition)
{
	SVector2i ViewportPosition = Viewport->GetViewportPosition();
	SVector2i ViewportSize = Viewport->GetViewportSize();
	float x = (2.0f * ((float)InScreenPosition.X - ViewportPosition.X)) / (float)ViewportSize.X - 1.0f;
	float y = 1.0f - (2.0f * ((float)InScreenPosition.Y - ViewportPosition.Y)) / (float)ViewportSize.Y;
	const SVector2 RayNds = {x, y};

	const SVector4 RayClip = SVector4(RayNds.X, RayNds.Y, -1.0f, 1.0f);
	
	SVector4 RayEye = Projection.GetInverse() * RayClip;
	RayEye = SVector4(RayEye.X, RayEye.Y, -1.0f, 0.0f);

	SVector RayWorld = SVector(View.GetInverse().ToGLM() * RayEye);

	RayWorld = RayWorld.Normalize();
	return RayWorld;
}

SVector CameraManager::ScreenToWorldPosition2D(SVector2i InScreenPosition)
{
	const SVector PlaneNormal = -CameraForward;
	const SVector MouseDirection = ScreenToWorldDirection(InScreenPosition);
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
void CameraManager::SetMVP(STransform InTransform, GLuint program)
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
				const float HalfWidth = static_cast<float>(Viewport->GetViewportSize().X) / fWindowScale;
				const float HalfHeight = static_cast<float>(Viewport->GetViewportSize().Y) / fWindowScale;
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
				ProjectionInfo.Width = static_cast<float>(Viewport->GetViewportSize().X);
				ProjectionInfo.Height = static_cast<float>(Viewport->GetViewportSize().Y);
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
