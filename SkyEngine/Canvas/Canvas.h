// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#pragma once

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class CEngineWindow;

// Could remove export if other projects should not make their own layers but used derived ones
class ENGINE_API CCanvas : public std::enable_shared_from_this<CCanvas>
{
public:
	CCanvas(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InDebugName = "Layer");
	virtual ~CCanvas();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	void OnEvent(CEvent& Event);
	virtual void OnRender() {}
	inline const std::string& GetName() const { return DebugName; }
	TWeakPointer<CEngineWindow> GetOwningWindow() const { return OwningWindow; }

protected:
	virtual bool OnMouseButtonPressed(int Button, int Mods);
	virtual bool OnMouseButtonReleased(int Button, int Mods);
	virtual bool OnMouseMoved(SVector2i MousePos);
	virtual bool OnMouseScrolled(float XOffset, float YOffset);
	virtual bool OnKeyPressed(int KeyCode, int Mods, int RepeatCount);
	virtual bool OnKeyTyped(int KeyCode, int Mods);
	virtual bool OnKeyReleased(int KeyCode, int Mods);
	virtual bool OnWindowResize(unsigned int Width, unsigned int Height);
	
	TWeakPointer<CEngineWindow> OwningWindow;
	std::string DebugName;

private:
	virtual bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	virtual bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	virtual bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	virtual bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	virtual bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	virtual bool OnKeyTypedEvent(CKeyTypedEvent& Event);
	virtual bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);
	virtual bool OnWindowResizeEvent(CWindowResizeEvent& Event);
	
};
