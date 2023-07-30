// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <map>

// OpenGL Library Includes //
#include <glew/glew.h>
#include <GLFW/glfw3.h>

// Engine Includes //
// #include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Math/Matrix.h"
#include "Math/Vector2.h"
#include "System/Utils.h"

// TODO: How does class affect this? 
enum class EProjectionMode
{
	Orthographic = 1,
	Perspective
};

class EngineWindow;
struct FTransform;

class ENGINE_API CameraManager
{
public:

	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 800;
	unsigned int VIEWPORT_X = 0;
	unsigned int VIEWPORT_Y = 0;
		
	Matrix4 View;
	Matrix4 Projection;

	void Init(int ScreenWidth, int ScreenHeight, Vector3 CamPos, Vector3 ForwardVec, Vector3 UpVec);

	void UpdateViewMatrix();

	virtual void SpectatorUpdate();

	glm::mat4 GetMVP(glm::mat4 model) const;
	void SetMVP(FTransform InTransform, GLuint program);

	void SwitchProjection(EProjectionMode InMode);

	EProjectionMode ReturnDimensionMode() { return ProjectionMode; };

	void SetWindowScale(float _fNewScale);

	float GetWindowScale() { return fWindowScale; }

	Vector3 ScreenToWorldDirection(Vector2 InScreenPosition);

	Vector3 ScreenToWorldPosition2D(Vector2 InScreenPosition);
	
	// Getters
	Vector3 GetCameraPosition() const { return CameraPosition; }
	Vector3 GetCameraForwardVector() const { return CameraForward; }
	Vector3 GetCameraUpVector() const { return CameraUp; }

	Vector3 GetCameraRightVector() const;

	void SetCameraForwardVector(Vector3 _Forward);
	
	void SetCameraPos(Vector3 NewPos);

	void MoveCamera(Vector3 _Movement);

	void EnableSpectatorControls(bool _bSpectatorControls);

	void ToggleSpectatorControls()
	{
		bUseSpectatorControls = !bUseSpectatorControls;
	}

	bool bSpectatorMovement = true;
	float MouseSensitivity = 0.15f;

	Pointer<EngineWindow> MainWindow;

protected:
	
private:
	// FPS CONTROLS
	bool bUseSpectatorControls = false;

	float CameraSpeed = 12.0f;
	GLfloat Yaw = 0.0f;
	GLfloat Pitch = 0.0f;

	Vector3 CameraPosition;
	Vector3 CameraForward;
	Vector3 CameraUp;

	EProjectionMode ProjectionMode = EProjectionMode::Perspective;

	// TODO: Check only for orthographic or apply to perspective
	float fWindowScale = 200;
	
	float fMaxViewClipping = 2000.0f;

	// Singleton
public:
	static CameraManager* GetInstance();

	static void DestoryInstance();

private:
	static CameraManager* m_pCamera;
	static std::map<int, CameraManager*> m_pCameras;

	CameraManager();

	~CameraManager();
	
	CameraManager(const CameraManager&); // Don't Implement
	void operator=(const CameraManager&); // Don't implement
};