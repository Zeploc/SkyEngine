// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "MathDefinitions.h"
#include "Rotator.h"
#include "Transform.h"
#include "Vector.h"

class ENGINE_API CMath
{
public:
	static bool DecomposeTransform(const glm::mat4& OutMatrix, SVector& Translation, SRotator& Rotation, SVector& Scale);
	static bool DecomposeTransform(const glm::mat4& OutMatrix, STransform& Transform);
};
