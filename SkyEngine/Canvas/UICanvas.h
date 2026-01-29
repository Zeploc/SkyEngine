// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Canvas/Canvas.h"

class CUIWidget;

class ENGINE_API CUICanvas : public CCanvas
{
public:
	CUICanvas(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InCanvasName);
	virtual ~CUICanvas();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;
	void AddWidget(TSharedPointer<CUIWidget> InWidget);

protected:
	// TODO: Override events based on widgets if blocking/handling

	std::vector<TSharedPointer<CUIWidget>> Widgets;
};
