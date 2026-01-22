// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "LogManager.h"

#include "Core/Application.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CLogManager::CLogManager()
{
	//LoadingMessage = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 }, 0, { 0.7f, 0.7f, 0.7f, 1.0f }, "Log Message", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::CENTER));;
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
CLogManager::~CLogManager()
{
}

void CLogManager::Init()
{
	DisplayMessage("Loading Game");
}

void CLogManager::Render()
{
	// LoadingMessage->DrawUIElement();
}

void CLogManager::LogInternal(ELogMessageType MessageType, const std::string& InMessage)
{
	switch (MessageType)
	{
	case ELogMessageType::Message:
		std::cout << InMessage << std::endl;
		break;
	case ELogMessageType::Warning:
	case ELogMessageType::Error:		
		std::cerr << InMessage << std::endl;
		break;
	default: ;
	}
}

void CLogManager::Log(ELogMessageType MessageType, const std::string& InMessage)
{
	std::string Prefix = "";
	if (MessageType != ELogMessageType::Message)
	{
		Prefix = (MessageType == ELogMessageType::Error ? "[ERROR]" : "[WARN]");
		Prefix +=  ": ";
	}
	
	
	LogInternal(MessageType, Prefix + InMessage);
}

void CLogManager::DisplayMessage(const std::string& InMessage)
{
	Log(ELogMessageType::Message, InMessage);
}

void CLogManager::DisplayWarning(const std::string& InMessage)
{
	Log(ELogMessageType::Warning, InMessage);
}

void CLogManager::DisplayError(const std::string& InMessage)
{
	Log(ELogMessageType::Error, InMessage);
}

TSharedPointer<CLogManager> CLogManager::Get()
{
	return GetApplication()->LogManager;
}
