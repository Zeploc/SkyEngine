// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

class CCanvas;
class CEngineWindow;
struct ImGuiContext;

class CCanvasManager
{
public:
	CCanvasManager();
	virtual ~CCanvasManager();
	void SetupCanvasManager(TPointer<CEngineWindow> InOwningWindow);
	/* Returns responded canvas */
	void OnEvent(CEvent& Event);

	void Update();
	void Render();

	/* Pushed onto the end of the layers, but before the overlays*/
	void PushLayer(CCanvas* InLayer);
	/* Always pushed on top*/
	void PushOverlay(CCanvas* InOverlay);
	void PopLayer(CCanvas* InLayer);
	void PopOverlay(CCanvas* InOverlay);

	std::vector<CCanvas*>::iterator begin() { return Canvases.begin(); }
	std::vector<CCanvas*>::iterator end() { return Canvases.end(); }

	ImGuiContext* GetGuiContext() const { return GuiContext; }
	CCanvas* GetCapturedLayer() const { return CapturedLayer; }
private:
	bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	bool OnKeyTypedEvent(CKeyTypedEvent& Event);
	bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);
	bool OnWindowResizeEvent(CWindowResizeEvent& Event);

	CCanvas* SendCanvasEvents(CEvent& Event);
	
	// TODO: Convert to shared pointers
	std::vector<CCanvas*> Canvases;
	unsigned int LayersInsertIndex = 0;
	TWeakPointer<CEngineWindow> OwningWindow;
	CCanvas* CapturedLayer = nullptr;
	// TODO: Active layer (selected for keyboard input to go to)
	
	ImGuiContext* GuiContext;

};
