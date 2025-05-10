// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"
#include "Math/Matrix.h"
#include "Math/Vector2.h"

class Camera;
class IFramebuffer;
class Scene;

class ENGINE_API CSceneRenderer
{
public:
	void Init(TAssetObjectPointer<Scene> InTargetScene, SVector2i InSize);
	void SizeChanged(SVector2i InSize);
	void Render();
	void SetSceneTarget(TAssetObjectPointer<Scene> InTargetScene);
	void LinkCamera(THardPointer<Camera> Camera);
	void UpdateOrthographicProjection(float Scale, float MaxViewClipping);
	void UpdatePerspectiveProjection(float FOV, float MaxViewClipping);

	THardPointer<IFramebuffer> GetFramebuffer() const { return Framebuffer; }
	
	// TODO: Create colour type
	SVector ClearColour = SVector(0.3f, 0.8f, 0.9f);
	
	Matrix4 GetView();
	Matrix4 GetProjection();
	TAssetObjectPointer<Scene> GetTargetScene() const { return TargetScene; }

protected:
	
	// TODO: Multiple frame buffers (multiple windows, etc)
	THardPointer<IFramebuffer> Framebuffer;

	TAssetObjectPointer<Scene> TargetScene;
	SVector2i ViewSize;
	
	Matrix4 View;
	Matrix4 Projection;
	THardPointer<Camera> LinkedCamera;

	// TODO: Enabled/disabled?
};
