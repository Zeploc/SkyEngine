// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Dependencies/ImGui/imgui.h"
#include "UI/UIWidget.h"

class CConsoleLog : public CUIWidget
{
public:
	CConsoleLog();
	void Clear();
	void AddLog(const char* fmt, ...);
	void DrawUI(const SCanvas& DrawCanvas) override;

protected:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;
};
