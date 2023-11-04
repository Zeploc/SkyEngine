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

void CLogManager::DisplayMessage(const std::string& InMessage)
{
	std::cout << InMessage << std::endl;
	// TODO: Log
	// if (LoadingMessage)
	// {
	// 	LoadingMessage->sText = InMessage;
	// }
}

void CLogManager::DisplayWarning(const std::string& InMessage)
{
	std::cerr << InMessage << std::endl;
}

void CLogManager::DisplayError(const std::string& InMessage)
{
	std::cerr << InMessage << std::endl;
}

TPointer<CLogManager> CLogManager::Get()
{
	return GetApplication()->LogManager;
}
