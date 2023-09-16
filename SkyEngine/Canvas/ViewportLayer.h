// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Canvas.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Math/Vector.h"
#include "Math/Vector2.h"

class CTexture;

class ENGINE_API CViewportLayer : public CCanvas
{
public:
	CViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow);
	~CViewportLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;

	virtual SVector2i GetViewportSize();
	virtual SVector2i GetViewportPosition();
	
protected:
	bool OnMouseButtonPressed(int Button, int Mods) override;
	bool OnMouseButtonReleased(int Button, int Mods) override;
	bool OnMouseMoved(SVector2i MousePos) override;
	bool OnMouseScrolled(float XOffset, float YOffset) override;
	bool OnKeyPressed(int KeyCode, int Mods, int RepeatCount) override;
	bool OnKeyTyped(int KeyCode, int Mods) override;
	bool OnKeyReleased(int KeyCode, int Mods) override;
	bool OnWindowResize(unsigned int Width, unsigned int Height) override;
	
	SVector SkyColour;
	TPointer<CTexture> ViewportTexture;
};
