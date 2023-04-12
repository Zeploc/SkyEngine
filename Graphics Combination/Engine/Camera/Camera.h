// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <map>

// OpenGL Library Includes //
#include <glew.h>
#include <glfw3.h>

// Engine Includes //
#include "Engine/Entity/2DParticleSystem.h"
#include "Engine/Entity/2DParticleSystem.h"
#include "Engine/Math/Matrix.h"
#include "Engine/System/Utils.h"

// TODO: How does class affect this? 
enum class EProjectionMode
{
	Orthographic = 1,
	Perspective
};

class Camera
{
public:

	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 800;
	unsigned int VIEWPORT_X = 0;
	unsigned int VIEWPORT_Y = 0;
		
	Matrix4 View;
	Matrix4 Projection;

	void Init(int ScreenWidth, int ScreenWidthheight, Vector3 CamPos, Vector3 ForwardVec, Vector3 UpVec);

	void Update();

	void SetMVP(FTransform InTransform, GLuint program);

	void SwitchProjection(EProjectionMode InMode);

	EProjectionMode ReturnDimensionMode() { return ProjectionMode; };

	void SetWindowScale(float _fNewScale);

	float GetWindowScale() { return fWindowScale; }

	void SetCameraForwardVector(Vector3 _Forward)
	{
		CameraForward = _Forward;
	}

	Vector3 ScreenToWorldDirection(Vector2 InScreenPosition);

	Vector3 ScreenToWorldPosition2D(Vector2 InScreenPosition);

	// Getters
	Vector3 GetCameraPosition() const { return CameraPosition; }
	Vector3 GetCameraForwardVector() const { return CameraForward; }
	Vector3 GetCameraUpVector() const { return CameraUp; }

	Vector3 GetCameraRightVector();

	void SetCameraPos(Vector3 NewPos) { CameraPosition = NewPos; }

	void MoveCamera(Vector3 _Movement);

	void EnableSpectatorControls(bool _bSpectatorControls);

	void ToggleSpectatorControls()
	{
		bUseSpectatorControls = !bUseSpectatorControls;
	}

	bool bSpectatorMovement = true;

	struct GLFWwindow* MainWindow;

protected:
	virtual void SpectatorControls();
	
private:
	// FPS CONTROLS
	bool bUseSpectatorControls = false;

	float MouseSensitivity = 0.15f;
	float cameraSpeed = 5.0f;
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
	static Camera* GetInstance();

	static void DestoryInstance();

private:
	static Camera* m_pCamera;
	static std::map<int, Camera*> m_pCameras;

	Camera();

	~Camera();

	Camera(const Camera&); // Don't Implement
	void operator=(const Camera&); // Don't implement
};
