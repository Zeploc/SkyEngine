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
	DisplayLogMessage("Loading Game");
}

void CLogManager::Render()
{
	// LoadingMessage->DrawUIElement();
}

void CLogManager::DisplayLogMessage(const std::string& InMessage)
{
	std::cout << InMessage << std::endl;
	// TODO: Log
	// if (LoadingMessage)
	// {
	// 	LoadingMessage->sText = InMessage;
	// }
}

void CLogManager::DisplayLogWarning(const std::string& InMessage)
{
	std::cerr << InMessage << std::endl;
}

void CLogManager::DisplayLogError(const std::string& InMessage)
{
	std::cerr << InMessage << std::endl;
}

TPointer<CLogManager> CLogManager::GetInstance()
{
	return GetApplication()->LogManager;
}
