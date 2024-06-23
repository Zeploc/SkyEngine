#include "EditorLogManager.h"

#include "EditorApp.h"
#include "UI/ConsoleLog.h"

void CEditorLogManager::LogInternal(ELogMessageType MessageType, const std::string& InMessage)
{
	CLogManager::LogInternal(MessageType, InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(MessageType ,std::format("{}\n", InMessage).c_str());
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
		// todo: store log type in each message
		EditorApp->ConsoleLog->AddLog(ELogMessageType::Message,std::format("{}\n", Message).c_str());
	}
	QueuedMessages.clear();
}
