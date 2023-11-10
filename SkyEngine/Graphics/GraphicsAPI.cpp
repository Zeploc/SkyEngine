// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GraphicsAPI.h"

#include "GL/GLAPI.h"
#include "System/LogManager.h"
#include "Vulkan/VulkanAPI.h"

TPointer<IGraphicsAPI> IGraphicsAPI::CreateGraphicsAPI(EGraphicsAPI APIType)
{
	TPointer<IGraphicsAPI> NewGraphicsApi = nullptr;
	switch (APIType)
	{
	case EGraphicsAPI::OPENGL:
		NewGraphicsApi = std::make_shared<CGLAPI>();
	case EGraphicsAPI::VULKAN:
		NewGraphicsApi = std::make_shared<CVulkanAPI>();
		break;
	case EGraphicsAPI::DIRECTX:
		break;
	default: ;
	}
	if (NewGraphicsApi)
	{
		NewGraphicsApi->Init();
	}
	else
	{
		CLogManager::GetInstance()->DisplayLogMessage("No valid Graphics API Set!");		
	}
	return NewGraphicsApi;
}
