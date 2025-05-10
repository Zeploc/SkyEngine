// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"


struct SSerializableVariable;

class CUIPresets
{
public:
	static void RenderVariableField(SSerializableVariable& Variable);
	
	/* If class filter is empty all assets displayed, returns true if a change of asset occurred */
	static bool RenderAssetObjectField(TAssetObjectPointer<>& AssetObject, std::string ClassFilter = std::string());
};
