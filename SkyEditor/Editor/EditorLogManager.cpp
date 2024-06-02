#include "EditorLogManager.h"

#include "EditorApp.h"
#include "UI/ConsoleLog.h"

void CEditorLogManager::DisplayMessage(const std::string& InMessage)
{
	CLogManager::DisplayMessage(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
	else
	{
		QueuedMessages.push_back(InMessage);
	}
}

void CEditorLogManager::DisplayWarning(const std::string& InMessage)
{
	CLogManager::DisplayWarning(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
	else
	{
		QueuedMessages.push_back(InMessage);
	}
}

void CEditorLogManager::DisplayError(const std::string& InMessage)
{
	CLogManager::DisplayError(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
	else
	{
		QueuedMessages.push_back(InMessage);
	}
}

void CEditorLogManager::ConsoleLogCreated()
{
	if (!EditorApp->ConsoleLog)
	{
		return;
	}
	for (std::string Message : QueuedMessages)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", Message).c_str());
	}
	QueuedMessages.clear();
}
