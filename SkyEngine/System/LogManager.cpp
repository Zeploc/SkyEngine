// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "LogManager.h"

#include "UI/Legacy/UIText.h"

// Library Includes //

// Static Variables //
TPointer<LogManager> LogManager::m_pLogManager;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
LogManager::LogManager()
{
	//LoadingMessage = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 }, 0, { 0.7f, 0.7f, 0.7f, 1.0f }, "Log Message", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::CENTER));;
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
LogManager::~LogManager()
{
}

void LogManager::Init()
{
	DisplayLogMessage("Loading Game");
}

void LogManager::Render()
{
	LoadingMessage->DrawUIElement();
}

void LogManager::DisplayLogMessage(const std::string& InMessage)
{
	std::cout << InMessage << std::endl;
	if (LoadingMessage)
	{
		LoadingMessage->sText = InMessage;
	}
}

void LogManager::DisplayLogWarning(const std::string& InMessage)
{
	std::cerr << InMessage << std::endl;
}

void LogManager::DisplayLogError(const std::string& InMessage)
{
	std::cerr << InMessage << std::endl;
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
TPointer<LogManager> LogManager::GetInstance()
{
	if (!m_pLogManager) // null or doesn't exist
	{
		m_pLogManager = TPointer<LogManager>(new LogManager());
	}
	return m_pLogManager;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void LogManager::DestoryInstance()
{
	m_pLogManager = nullptr;
}
