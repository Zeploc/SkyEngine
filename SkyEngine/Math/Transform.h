// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Rotator.h"
#include "Vector.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

struct ENGINE_API STransform
{
	
	SVector Position;
	SRotator Rotation;
	SVector Scale;

	std::string ToString() const;
	bool FromMatrix(glm::mat4 Matrix);
	void FromString(std::string sTransform);
	
	glm::mat4 GetModelMatrix() const;
	
	friend std::ostream& operator<<(std::ostream& os, const STransform& InTransform);
	friend std::istream& operator>>(std::istream& is, STransform& OutTransform);
};
