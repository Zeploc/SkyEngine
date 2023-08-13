#include "EditorViewportLayer.h"

#include "UI/UIWidget.h"

CEditorViewportLayer::CEditorViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow)
	: CViewportLayer(InOwningWindow)
{
}

void CEditorViewportLayer::OnUpdate()
{
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->Update();
	}	
	
	CViewportLayer::OnUpdate();
}

void CEditorViewportLayer::OnRender()
{	
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->DrawUI();
	}
	
	CViewportLayer::OnRender();
}

void CEditorViewportLayer::AddViewportWidget(TPointer<CUIWidget> InWidget)
{	
	Widgets.push_back(InWidget);	
	InWidget->SetOwningLayer(this);
}
