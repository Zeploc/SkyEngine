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
#include "Math/Matrix.h"
#include "Math/Vector2.h"
#include "System/Utils.h"

// TODO: How does class affect this? 
enum class EProjectionMode
{
	Orthographic = 1,
	Perspective
};

class CViewportLayer;
struct STransform;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API CameraManager
{
public:		
	Matrix4 View;
	Matrix4 Projection;

	void Init(CViewportLayer* InViewportLayer, SVector CamPos, SVector ForwardVec, SVector UpVec);

	void UpdateViewMatrix();

	virtual void SpectatorUpdate();

	glm::mat4 GetMVP(glm::mat4 model) const;
	void SetMVP(STransform InTransform, GLuint program);

	void SwitchProjection(EProjectionMode InMode);

	EProjectionMode ReturnDimensionMode() { return ProjectionMode; };

	void SetWindowScale(float _fNewScale);

	float GetWindowScale() { return fWindowScale; }

	SVector ScreenToWorldDirection(SVector2i InScreenPosition);

	SVector ScreenToWorldPosition2D(SVector2i InScreenPosition);
		
	// Getters
	SVector GetCameraPosition() const { return CameraPosition; }
	SVector GetCameraForwardVector() const { return CameraForward; }
	SVector GetCameraUpVector() const { return CameraUp; }

	SVector GetCameraRightVector() const;

	void SetCameraForwardVector(SVector _Forward);
	
	void SetCameraPos(SVector NewPos);

	void MoveCamera(SVector _Movement);
	SVector2i GetScreenCenter() const;

	void EnableSpectatorControls(bool _bSpectatorControls);

	void ToggleSpectatorControls()
	{
		bUseSpectatorControls = !bUseSpectatorControls;
	}

	bool bSpectatorMovement = true;
	float MouseSensitivity = 0.15f;

protected:
	CViewportLayer* Viewport;
	
private:
	// FPS CONTROLS
	bool bUseSpectatorControls = false;

	SVector2i PreviousMousePosition;

	float CameraSpeed = 12.0f;
	float Yaw = 0.0f;
	float Pitch = 0.0f;

	TVector3<float> CameraPosition;
	TVector3<float> CameraForward;
	TVector3<float> CameraUp;

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