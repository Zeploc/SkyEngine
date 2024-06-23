// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Math/Vector.h"
#include "Math/Vector2.h"

class CSceneRenderer;
class CTexture;
class CWorldManager;
class Camera;

class ENGINE_API CViewportCanvas : public CCanvas
{
public:
	CViewportCanvas(TWeakPointer<CEngineWindow> InOwningWindow);
	~CViewportCanvas() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;

	virtual void OnViewportResize();

	// TODO: Resolve to not be needed
	void SetupCamera();
	void SetCamera(TPointer<Camera> NewCamera);

	virtual SVector2i GetViewportSize();
	virtual SVector2i GetViewportPosition();
	TPointer<Camera> GetViewportCamera() const { return ViewportCamera;	}
	TPointer<CSceneRenderer> GetSceneRenderer() const { return SceneRenderer; }
	SVector ScreenToWorldDirection(SVector2i InScreenPosition);
	SVector ScreenToWorldPosition2D(SVector2i InScreenPosition);
	
protected:
	bool OnMouseButtonPressed(int MouseButton, int Mods) override;
	bool OnMouseButtonReleased(int MouseButton, int Mods) override;
	bool OnMouseMoved(SVector2i MousePos) override;
	bool OnMouseScrolled(float XOffset, float YOffset) override;
	bool OnKeyPressed(int KeyCode, int Mods, int RepeatCount) override;
	bool OnKeyTyped(int KeyCode, int Mods) override;
	bool OnKeyReleased(int KeyCode, int Mods) override;
	bool OnWindowResize(unsigned int Width, unsigned int Height) override;
	
	SVector SkyColour;
	TPointer<CWorldManager> WorldManager; 
	SVector2i ViewportSize;
	TPointer<CSceneRenderer> SceneRenderer;
	TPointer<Camera> ViewportCamera;
};
