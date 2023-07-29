// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "LogManager.h"

#include "UI/UIText.h"

// Library Includes //
#include <iostream>

// Static Variables //
Pointer<LogManager> LogManager::m_pLogManager;

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

void LogManager::DisplayLogMessage(std::string _Message)
{
	std::cout << _Message << std::endl;
	if (LoadingMessage)
	{
		LoadingMessage->sText = _Message;
	}
	//glutPostRedisplay();
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
Pointer<LogManager> LogManager::GetInstance()
{
	if (!m_pLogManager) // null or doesn't exist
	{
		m_pLogManager = Pointer<LogManager>(new LogManager());
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
