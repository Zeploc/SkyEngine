#include "ConsoleLog.h"

#include "Dependencies/ImGui/imgui.h"

CConsoleLog::CConsoleLog(TWeakPointer<CEngineWindow> InOwningWindow)
: CUICanvas(InOwningWindow, "Console Log")
{
	StartingSize = {500, 400};
	// TODO: Get existing log items and add to initial
	// TODO: Make button to open and remove
	bCanClose = false;
}

void CConsoleLog::Clear()
{
	Buf.clear();
	LineOffsets.clear();
}

void CConsoleLog::AddLog(ELogMessageType MessageType, const char* fmt, ...) IM_FMTARGS(2)
{
	// TODO: Read line for error
	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt, args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
	{
		if (Buf[old_size] == '\n')
		{
			LineOffsets.push_back(old_size);
		}
	}
	ScrollToBottom = true;
}

void CConsoleLog::OnRender()
{
	// TODO: Convert to widget and use base render
	if (ImGui::Button("Clear"))
		Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (copy)
		ImGui::LogToClipboard();

	if (Filter.IsActive())
	{
		const char* buf_begin = Buf.begin();
		const char* line = buf_begin;
		for (int line_no = 0; line != NULL; line_no++)
		{
			const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
			if (Filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : NULL;
		}
	}
	else
	{
		ImGui::TextUnformatted(Buf.begin());
	}

	if (ScrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;
	ImGui::EndChild();
}
