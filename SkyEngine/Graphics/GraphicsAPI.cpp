// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GraphicsAPI.h"

#include "GL/GLAPI.h"
#include "System/LogManager.h"

TPointer<IGraphicsAPI> IGraphicsAPI::CreateGraphicsAPI(EGraphicsAPI APIType)
{
	switch (APIType)
	{
	case EGraphicsAPI::OPENGL:
		return std::make_shared<IGLAPI>();
	case EGraphicsAPI::VULKAN:
		break;
	default: ;
	}
	LogManager::GetInstance()->DisplayLogMessage("No valid Graphics API Set!");
	return nullptr;
}
